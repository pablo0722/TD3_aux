/************************************
 *          INCLUDES                *
 ************************************/
// C headers
#include <stdlib.h>     // NULL
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
#include <pthread.h>        // pthread_create(), pthread_self()
#include <sys/stat.h>

// Common headers
#include "socket.h"     // CLIENT_ALIVE_TIMEOUT
#include "debug.h"          // DEBUG

// Project headers
#include "my_server.h"  // SERVER_PORT
#include "basics.h"     // SERVER_PORT










/************************************
 *          FORWARDS                *
 ************************************/
void *thread_read_func(void *arg);
void *thread_write_func(void *arg);










/************************************
 *          VARIABLES               *
 ************************************/
// extern
    extern int client_fd;
// static
    static bool terminate_child = false;










/************************************
 *          THREAD_FUNC             *
 ************************************/
void create_thread(t_client *client)
{
    pthread_t thread;
    pthread_attr_t attr;

    if (pthread_attr_init(&attr) != 0)
    {
        printf ("Error on pthread_attr_init()\n");

        exit (RET_MAIN_ERROR_THREAD_ATTR_INIT);
    }
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
    {
        printf ("Error on pthread_attr_setdetachstate()\n");

        exit (RET_MAIN_ERROR_THREAD_ATTR_DETACH);
    }

    if (pthread_create (&thread, &attr, thread_read_func, client) != 0)
    {
        printf ("Error on pthread_create()\n");

        exit (RET_MAIN_ERROR_THREAD_CREATE);
    }

    if (pthread_create (&thread, &attr, thread_write_func, client) != 0)
    {
        printf ("Error on pthread_create()\n");

        exit (RET_MAIN_ERROR_THREAD_CREATE);
    }

    pthread_attr_destroy(&attr);
}










void *thread_read_func(void *arg)
{
    t_client *  client = arg;
    char        buffer [BUFFER_LEN] = {0};



    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(client->client_fd, &rfds);

    tv.tv_sec = CLIENT_ALIVE_TIMEOUT;
    tv.tv_usec = 0;

    while (!terminate_child)
    {
        while (retval = select (client->client_fd + 1, &rfds, NULL, NULL, &tv),
            errno == EINTR);

        if (retval == -1)
        {
            perror("Error select()");

            client->close_thread = true;

            break;
        }
        else if (retval)
        {
            // FD_ISSET(client->client_fd, &rfds) == true

            size_t read_len;
            if ((read_len = read(client->client_fd, buffer, BUFFER_LEN - 1)) <= 0)
            {
                client->close_thread = true;

                break;
            }

            if (strstr (buffer, MSG_SEND_TIME))
            {
                client->send_time = atoi(buffer + strlen(MSG_SEND_TIME));
            }

            LOG("recibi: %s\n", buffer);
        }
        else
        {
            // timeout
            
            client->close_thread = true;

            LOG("Timeout\n");

            break;
        }


        FD_ZERO(&rfds);
        FD_SET(client->client_fd, &rfds);

        tv.tv_sec = CLIENT_ALIVE_TIMEOUT;
        tv.tv_usec = 0;
    }

    printf ("Cierra cliente\n");

    close (client->client_fd);


    return NULL;
}










void *thread_write_func(void *arg)
{
    t_client *  client = arg;
    char        buffer [BUFFER_LEN] = {0};

    int fd = open("/dev/i2c_td3_dev", O_RDONLY);
    int buf;

    if (fd < 0)
    {
        perror("/dev/i2c_td3");
        client->close_thread = true;

        return NULL;
    }

    while (!client->close_thread)
    {
        read(fd, &buf, sizeof(int));
        sprintf (buffer, "%s%d", MSG_TEMPERATURE, buf);
        write   (client->client_fd, buffer, strlen(buffer) + 1);

        sleep (client->send_time);
    }


    return NULL;
}