/************************************
 *          INCLUDES                *
 ************************************/
// C headers
#include <stdlib.h>             // atoi()
#include <stdio.h>              // printf()
#include <signal.h>             // SIGINT, struct sigaction, sigemptyset(), sigaction()
#include <netinet/in.h>         // struct sockaddr_in
#include <unistd.h>             // close()
#include <sys/wait.h>           // waitpid()
#include <stdint.h>             // uint64_t

// Common headers
#include "socket.h"             // socket_client_connect()
#include "dynamic_vector.h"     // t_dynamic_vector, dynamic_vector_create(), dynamic_vector_add(), dynamic_vector_remove(), dynamic_vector_free()
#include "debug.h"              // DEBUG

// Project headers
#include "my_server.h"










/************************************
 *          DEFINE                  *
 ************************************/
#define BACKLOG     2










/************************************
 *          FORWARDS                *
 ************************************/
static void sigint_handler (int signal);










/************************************
 *          VARIABLES               *
 ************************************/
int sock_fd;










/************************************
 *          MAIN                    *
 ************************************/
int main(int argc, char *argv[])
{
    // Uso thread para que el hijo no mande SIGCHLD
    // cuando muera y cancele accept().



    if (argc != 2)
    {
        printf("USE: ./server [port]\n");

        return RET_MAIN_INSUFICIENT_ARGS;
    }

    int port = atoi(argv[1]);
    if (port <= 0)
    {
        printf ("port must be greater than 0\n");

        return RET_MAIN_INVALID_ARGS;
    }

    sock_fd = socket_server_open(port, BACKLOG);
    if (sock_fd < 0)
    {
        perror("socket_open");

        return RET_MAIN_ERROR_SOCKET_OPEN;
    }

    struct sigaction sigact;
    sigemptyset (&sigact.sa_mask);
    sigact.sa_handler = sigint_handler;
    sigaction(SIGINT, &sigact, NULL);


    t_dynamic_vector clients;
    t_client client;

    client.sock_fd = sock_fd;
    client.close_thread = false;
    client.send_time = SEND_TIME;
    
    dynamic_vector_create (&clients, sizeof(t_client));


    while (1)
    {
        if (clients.num_elements >= MAX_CLIENTS)
        {
            printf ("Max clients (%d) reached. Wating vacancy...\n", MAX_CLIENTS);
        }

        while (clients.num_elements >= MAX_CLIENTS)
        {
            sleep(10);
        }

        client.client_fd = socket_server_accept(sock_fd, client.ip);

        if (client.client_fd < 0)
        {
            perror ("Error accept()");

            kill(0, SIGINT); // Cierra conexiones incluyendo la de todos los hijos
        }

        printf ("%s conected\n", client.ip);

        dynamic_vector_add (&clients, &client);

        create_thread(clients.elements[clients.num_elements - 1].element);
    }

    close (sock_fd);
    
    printf ("\nCerrado correctamente\n");

    return RET_MAIN_OK;
}










static void sigint_handler (int signal)
{
    close (sock_fd);
    
    printf ("\nCerrado correctamente\n");

    exit (RET_MAIN_OK);
}
