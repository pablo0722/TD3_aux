/************************************
 *          INCLUDES                *
 ************************************/
// C headers
#include <stdlib.h>     // NULL, exit()
#include <stdio.h>      // printf()
#include <signal.h>     // SIGINT, struct sigaction, sigemptyset(), sigaction()
#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()
#include <sys/select.h> // select(), FD_ZERO, FD_SET
#include <sys/time.h>   // struct timeval
#include <stdint.h>     // uint64_t
#include <sys/types.h>
#include <string.h>     // strlen(), strncat()
#include <errno.h>      // errno
#include <pthread.h>    // pthread_create(), pthread_self()
#include <sys/stat.h>

// Common headers
#include "socket.h"     // CLIENT_ALIVE_TIMEOUT
#include "debug.h"      // DEBUG
#include "basics.h"     // bool, handle_error_en()
#include "IPC.h"        // xxx_semaphore

// Project headers
#include "my_server.h"  // MAX_CLIENTS, SEND_TIME, BUFFER_LEN, t_client, enum E_RET_MAIN
#include "thread.h"     // create_thread(), enum E_RET_THREAD










/************************************
 *          THREAD_FUNC             *
 ************************************/
void *thread_read_func(void *arg)
{
    t_client *  client = arg;
    char        buffer [BUFFER_LEN] = {0};

    fd_set rfds;
    struct timeval tv;
    int retval;

    LOG ("Openning thread read\n");
    

    while (!client->read_thread_close)
    {
        do
        {
            FD_ZERO(&rfds);
            FD_SET(client->client_fd, &rfds);

            tv.tv_sec = CLIENT_ALIVE_TIMEOUT;
            tv.tv_usec = 0;
            retval = select (client->client_fd + 1, &rfds, NULL, NULL, &tv);
        } while (errno == EINTR && retval == -1);

        if (retval == -1)
        {
            perror("Error select()");

            close (client->client_fd);
            client->read_thread_close  = true;
            client->write_thread_close = true;

            break;
        }
        else if (retval)
        {
            // FD_ISSET(client->client_fd, &rfds) == true

            size_t read_len;
            if ((read_len = read(client->client_fd, buffer, BUFFER_LEN - 1)) <= 0)
            {
                close (client->client_fd);
                client->read_thread_close  = true;
                client->write_thread_close = true;

                break;
            }
            else
            {
                if (strstr (buffer, MSG_SEND_TIME))
                {
                    client->send_time = atoi(buffer + strlen(MSG_SEND_TIME));
                }

                //LOG("recibi(%d): %s\n", client->client_fd, buffer);
            }
        }
        else
        {
            // timeout

            LOG("Timeout\n");

            close (client->client_fd);
            client->read_thread_close  = true;
            client->write_thread_close = true;

            break;
        }
    }


    LOG ("Closing thread read\n");

    // Me aseguro que el write_thread haya terminado
    lock_semaphore (client->sem_id_write_running);
    unlock_semaphore (client->sem_id_write_running);

    lock_semaphore (*client->sem_id_close_all);

    dynamic_vector_remove_by_address (client->clients, client);

    LOG ("Thread read closed\n");


    return NULL;
}










void *thread_write_func(void *arg)
{
    t_client *  client = arg;
    char        date [BUFFER_LEN] = {0};
    char        msg [BUFFER_LEN] = {0};

    t_buffer *  buffer_actual = client->buffer_B;

    LOG ("Openning thread write\n");

    while (!client->write_thread_close)
    {
        if (buffer_actual == client->buffer_A)
        {
            buffer_actual = client->buffer_B;
        }
        else
        {
            buffer_actual = client->buffer_A;
        }

        lock_semaphore (buffer_actual->sem_id);

        date[0] = '\0';
        strftime (date, BUFFER_LEN, "%d-%m-%y",   &buffer_actual->date);
        snprintf (msg, BUFFER_LEN, "%s%s,%d", MSG_LIGHT, date, buffer_actual->value);
        if (write    (client->client_fd, msg, strlen(msg) + 1) <= 0)
        {
            close (client->client_fd);
            client->read_thread_close  = true;
            client->write_thread_close = true;

            break;
        }
        LOG      ("envio (%d): \"%s\"\n", client->client_fd, msg);

        unlock_semaphore (buffer_actual->sem_id);

        sleep (client->send_time);
    }


    LOG ("Closing thread write\n");

    lock_semaphore (*client->sem_id_close_all);
    unlock_semaphore (client->sem_id_write_running);

    LOG ("Thread write closed\n");



    return NULL;
}