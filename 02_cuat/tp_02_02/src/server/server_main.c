/************************************
 *          INCLUDES                *
 ************************************/
// C headers
#include <stdlib.h>             // NULL, exit(), atoi()
#include <stdio.h>              // LOG()
#include <signal.h>             // SIGINT, struct sigaction, sigemptyset(), sigaction()
#include <netinet/in.h>         // struct sockaddr_in
#include <unistd.h>             // close()
#include <sys/wait.h>           // waitpid()
#include <stdint.h>             // uint64_t
#include <pthread.h>
#include <fcntl.h>              // open()
#include <sys/types.h>
#include <sys/stat.h>

// Common headers
#include "socket.h"             // socket_client_connect()
#include "dynamic_vector.h"     // t_dynamic_vector, dynamic_vector_create(), dynamic_vector_add(), dynamic_vector_remove(), dynamic_vector_free()
#include "debug.h"              // DEBUG
#include "basics.h"     		// bool, handle_error_en()
#include "IPC.h"                // xxx_semaphore()

// Project headers
#include "my_server.h"          // MAX_CLIENTS, SEND_TIME, BUFFER_LEN, t_client, enum E_RET_MAIN
#include "thread.h"             // create_thread(), , enum E_RET_THREAD










/************************************
 *          DEFINES                 *
 ************************************/
#define BACKLOG         2
#define ACCEPT_TIMEOUT  3










/************************************
 *          FORWARDS                *
 ************************************/
static void     close_all_clients   (void *element);
static void *   thread_device       (void *arg);
static void *   thread_accept       (void *arg);










/************************************
 *          MAIN                    *
 ************************************/
int main(int argc, char *argv[])
{
    // Uso thread para que el hijo no mande SIGCHLD
    // cuando muera y cancele accept().

	sigset_t set;
	int s, sig;
    t_thread_arg thread_arg;



    if (argc != 2)
    {
        LOG("USE: ./server [port]\n");

        return RET_MAIN_INSUFICIENT_ARGS;
    }

    int port = atoi(argv[1]);
    if (port <= 0)
    {
        LOG ("port must be greater than 0\n");

        return RET_MAIN_INVALID_ARGS;
    }

    thread_arg.accept_thread_close = false;
    thread_arg.device_thread_close = false;

    thread_arg.sem_id_close_all     = create_semaphore();
    thread_arg.buffer_A.sem_id      = create_semaphore();
    thread_arg.buffer_B.sem_id      = create_semaphore();

    lock_semaphore (thread_arg.sem_id_close_all);
    
    thread_arg.sock_fd = socket_server_open(port, BACKLOG);
    if (thread_arg.sock_fd < 0)
    {
        perror("socket_open");

        return RET_MAIN_ERROR_SOCKET_OPEN;
    }

	LOG ("Blocking signals... ");
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGPIPE);
	if ((s = pthread_sigmask(SIG_BLOCK, &set, NULL)) != 0)
	{
        handle_error_en(s, "pthread_sigmask");
        close (thread_arg.sock_fd);

        exit (RET_MAIN_ERROR_BLOCK_SIGNAL);
    }
    signal(SIGUSR1, SIG_IGN);
    LOG ("OK\n");

    // El open del device es bloqueante por lo que no lo puedo esperar (Abro sin dar semaforo)
    // unlock_semaphore (thread_arg.sem_id_close_all);
    if (create_thread (NULL, &thread_device, (void *) &thread_arg) != 0)
    {
        LOG ("Error on pthread_create() thread_device\n");
        close (thread_arg.sock_fd);

        exit (RET_MAIN_ERROR_THREAD_CREATE);
    }

    unlock_semaphore (thread_arg.sem_id_close_all);
    if (create_thread (NULL, &thread_accept, (void *) &thread_arg) != 0)
    {
		LOG ("Error on pthread_create() thread_accept\n");
        close (thread_arg.sock_fd);

        exit (RET_MAIN_ERROR_THREAD_CREATE);
    }

	sigemptyset(&set);
	sigaddset(&set, SIGINT);

    LOG ("\"Ctrl + C\" Para cerrar\n");
    do
    {
        s = sigwait(&set, &sig);
        if (s != 0)
        {
            handle_error_en(s, "sigwait");

            break;
        }
    } while (sig != SIGINT);


    LOG ("\nSIGINT catched\n");


    thread_arg.accept_thread_close = true;
    thread_arg.device_thread_close = true;


    // Libera al accept() del thread_accept
    close (thread_arg.sock_fd);


    dynamic_vector_do_for_each(&thread_arg.clients, close_all_clients);

    LOG ("Waiting threads to close\n");
    wait_for_locked_semaphore (thread_arg.sem_id_close_all);
    LOG ("Threads closed\n");

    dynamic_vector_free_all (&thread_arg.clients);


    LOG ("Close main_thread OK\n");

    exit (RET_MAIN_OK);
}










static void close_all_clients(void *element)
{
    t_client *client = (t_client *) element;

    client->read_thread_close  = true;
    client->write_thread_close = true;

    LOG ("closing client fd %d\n", client->client_fd);

    shutdown (client->client_fd, SHUT_RD);
}










static void * thread_accept (void *arg)
{
    t_thread_arg *thread_arg = (t_thread_arg *) arg;
    t_client * client_arg;

    LOG ("Openning thread accept\n");

    dynamic_vector_create (&thread_arg->clients, sizeof(t_client), MAX_CLIENTS);
    t_client client;

    client.clients              = &thread_arg->clients;
    client.sock_fd              = thread_arg->sock_fd;
    client.send_time            = SEND_TIME;
    client.read_thread_close    = false;
    client.write_thread_close   = false;
    client.buffer_A             = &thread_arg->buffer_A;
    client.buffer_B             = &thread_arg->buffer_B;
    client.sem_id_close_all     = &thread_arg->sem_id_close_all;

	LOG ("Waiting clients... \n");
    
    while (!thread_arg->accept_thread_close)
    {
        dynamic_vector_wait_for_vacancy (&thread_arg->clients);

        
        client.client_fd = 
            socket_server_accept_timed(thread_arg->sock_fd, client.ip, ACCEPT_TIMEOUT);

        if (client.client_fd != -RET_SOCKET_ACCEPT_TIMEOUT)
        {
            if (client.client_fd < 0)
            {
                LOG ("client.client_fd: %d\n", client.client_fd);
                perror ("Error accept()"); 

                kill(0, SIGINT); // envio signal al thread principal para cerrar todo
                break;
            }


            client.sem_id_write_running = create_semaphore();
            lock_semaphore (client.sem_id_write_running);


            LOG ("%s conected\n", client.ip);


            client_arg = dynamic_vector_add (&thread_arg->clients, &client);


            unlock_semaphore (thread_arg->sem_id_close_all);
            if (create_thread (NULL, thread_read_func, client_arg) != 0)
            {
                LOG ("Error on pthread_create() thread_read_func\n");

                kill(0, SIGINT); // envio signal al thread principal para cerrar todo
                break;
            }

            unlock_semaphore (thread_arg->sem_id_close_all);
            if (create_thread (NULL, thread_write_func, client_arg) != 0)
            {
                LOG ("Error on pthread_create() thread_write_func\n");

                kill(0, SIGINT); // envio signal al thread principal para cerrar todo
                break;
            }
        }
    }


    LOG ("Closing thread accept\n");

    dynamic_vector_wait_for_empty(&thread_arg->clients);

    lock_semaphore (thread_arg->sem_id_close_all);

    LOG ("Thread accept closed\n");

    return NULL;
}










void *thread_device(void *arg)
{
    t_thread_arg *  thread_arg          = (t_thread_arg *) arg;

    bool        first_open = true;
    int         fd;
    time_t      aux_time;
    t_buffer *  buffer_actual = &thread_arg->buffer_A;


    LOG ("Openning thread device\n");


    while (!thread_arg->device_thread_close)
    {
        first_open = true;
        lock_semaphore (thread_arg->buffer_A.sem_id);
        buffer_actual = &thread_arg->buffer_B;
        while (thread_arg->clients.num_elements == 0) sleep(1); // Espero clientes

        if (!thread_arg->device_thread_close)
        {
            fd = open("/dev/i2c_td3_dev", O_RDONLY);
            if (fd < 0)
            {
                perror("/dev/i2c_td3_dev");

                close (fd);

                kill(0, SIGINT); // envio signal al thread principal para cerrar todo
                break;
            }

            while (thread_arg->clients.num_elements > 0)
            {
                if (buffer_actual == &thread_arg->buffer_A)
                {
                    buffer_actual = &thread_arg->buffer_B;
                }
                else
                {
                    buffer_actual = &thread_arg->buffer_A;
                }

                if (!first_open)
                {
                    lock_semaphore (buffer_actual->sem_id);
                }

                first_open = false;

                time(&aux_time);
                buffer_actual->date = *localtime(&aux_time);

                read  (fd, &buffer_actual->value, sizeof(int));

                unlock_semaphore (buffer_actual->sem_id);
            }

            close (fd);
        }
    }


    LOG ("Closing thread device\n");

    // El open del device es bloqueante por lo que no lo puedo esperar (Cierro sin devolver semaforo)
    // lock_semaphore (thread_arg->sem_id_close_all);

    LOG ("Thread device closed\n");

    return NULL;
}
