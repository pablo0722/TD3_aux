/************************************
 *			INCLUDES				*
 ************************************/
// C headers
#include <stdio.h>          // printf()
#include <unistd.h>         // read(), write(), close()
#include <signal.h>         // SIGINT, SIGUSR1, struct sigaction, sigemptyset(), sigaction()
#include <string.h>         // strlen()
#include <stdlib.h>         // atoi()
#include <pthread.h>        // pthread_create(), pthread_self()
#include <sys/select.h>     // select(), FD_ZERO, FD_SET
#include <sys/time.h>       // struct timeval
#include <errno.h>			// errno
#include <sys/types.h>

// Common headers
#include "socket.h"         // socket_client_connect()
#include "dynamic_vector.h" // t_dynamic_vector, dynamic_vector_create(), dynamic_vector_add(), dynamic_vector_remove(), dynamic_vector_free()
#include "my_client.h"      // t_server
#include "common.h"         // MSG_SEND_TIME, MSG_IM_ALIVE, MSG_TEMPERATURE
#include "debug.h"          // DEBUG
#include "basics.h"     	// bool, handle_error_en()

// Project headers
#include "thread.h"     	// create_thread(), enum E_RET_THREAD
#include "IPC.h"     		// xxx_semaphore()










/************************************
 *			FORWARDS				*
 ************************************/
static void *thread_write_func (void *arg);
static void *thread_read_func  (void *arg);










/************************************
 *				MAIN				*
 ************************************/
void test_servers(t_dynamic_vector *servers)
{
	FILE *fp = fopen("assets/servidores.txt", "r");
	char ip[20];
	char port[10];
	char poll_time[10];
	t_server server_info;

	if (!fp)
	{
		perror("assets/servidores.txt");
		exit (-1);
	}


	dynamic_vector_create(servers, sizeof(t_server), -1); // -1: sin limite


	while(fscanf(fp, "%s %s %s", ip, port, poll_time) != EOF)
	{
		LOG ("trying to connect to %s:%s... ", ip, port);
		int sock_fd = socket_client_connect(ip, atoi(port));

		if (sock_fd >= 0)
		{
			LOG ("OK\n");
			strcpy(server_info.ip, ip);
			strcpy(server_info.port, port);
			server_info.sock_fd = sock_fd;
			server_info.poll_time = atoi (poll_time);
			server_info.thread_main = pthread_self();
			dynamic_vector_add(servers, &server_info);
		}
		else
		{
			LOG ("Error\n");
		}
	}
}










void create_threads(t_dynamic_vector *servers)
{
	int i;

	if (servers->num_elements == 0)
	{
		LOG ("No servers found\n");

		exit (-2);
	}


	for (i = 0; i < servers->num_elements; i++)
	{
		t_server *server = (t_server *) servers->elements[i].element;
		
		server->thread_closing      = false;
		server->thread_read_closed  = false;
		server->recieved            = false;
		server->buffer[0]           = '\0';

		server->servers = servers;

		if (create_thread (NULL, thread_write_func, server) != RET_THREAD_OK)
		{
			LOG ("Error on pthread_create()\n");
			exit (RET_MAIN_ERROR_THREAD_CREATE);
		}

		if (create_thread (NULL, thread_read_func, server) != 0)
		{
			LOG ("Error on pthread_create()\n");
			exit (RET_MAIN_ERROR_THREAD_CREATE);
		}
	}
}










static void *thread_write_func (void *arg)
{
	t_server *server = arg;

	char buffer[BUFFER_LEN];
	sprintf (buffer, "%s%d", MSG_SEND_TIME, server->poll_time);
	write (server->sock_fd, buffer, strlen(buffer) + 1);
	LOG("1st write: %s\n", buffer);

	while (!server->thread_closing)
	{
		if(sleep (CLIENT_ALIVE_TIMEOUT / 5) == 0) // sleep devuelve el time_left cuando fue interrumpido
		{
			if (write (server->sock_fd, MSG_IM_ALIVE, strlen(MSG_IM_ALIVE) + 1) == -1)
			{
				server->thread_closing = true;
			}
			else
			{
				LOG("write (%d): %s\n", server->sock_fd, MSG_IM_ALIVE);
			}
		}
	}
	LOG("sale write\n");

	close (server->sock_fd);

	// Espero que termine el thread de lectura
	while (!server->thread_read_closed) sleep(1);

	dynamic_vector_remove_by_address (server->servers, server);

	pthread_kill (server->thread_main, SIGUSR1);

	server->thread_write_closed = true;



	LOG("salio write\n");
	return NULL;
}










static void *thread_read_func (void *arg)
{
    fd_set 			rfds;
    struct timeval 	tv;
	t_server *  	server = arg;
	char        	aux_buffer [BUFFER_LEN];
	char *			msg = NULL;
    int 			retval;

    FD_ZERO(&rfds);
    FD_SET(server->sock_fd, &rfds);

	server->thread_read_closed = false;

	while (!server->thread_closing)
	{
        do
		{
			FD_ZERO(&rfds);
			FD_SET(server->sock_fd, &rfds);

			tv.tv_sec = READ_TIME;
			tv.tv_usec = 0;
			retval = select (server->sock_fd + 1, &rfds, NULL, NULL, &tv);
		} while	(errno == EINTR && retval == -1);
				
        if (retval == -1)
        {
            perror("Error select()");

            server->thread_closing = true;

            break;
        }
        else if (retval)
		{
			if (read(server->sock_fd, aux_buffer, BUFFER_LEN - 1) <= 0)
			{
				perror ("Error read()");

				server->thread_closing = true;
				break;
			}


			if (!server->thread_closing)
			{		
				if ((msg = strstr(aux_buffer, MSG_LIGHT)))
				{
					LOG ("Dia: %d, ",   atoi(msg + strlen(MSG_LIGHT)));
					LOG ("Mes: %d, ",   atoi(msg + strlen(MSG_LIGHT)+3));
					LOG ("Año: %d, ",   atoi(msg + strlen(MSG_LIGHT)+6));
					LOG ("Luz: %d\n\n", atoi(msg + strlen(MSG_LIGHT)+9));
				}
			}
		}
		else
		{
			// TIMEOUT
		}
	}
	LOG("sale read\n");

	close (server->sock_fd);

	server->thread_read_closed = true;


	LOG("salio read\n");

	return NULL;
}
