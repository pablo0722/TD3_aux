/************************************
 *			INCLUDES				*
 ************************************/
// C headers
#include <stdio.h>              // printf()
#include <unistd.h>             // read(), write(), close()
#include <signal.h>             // SIGUSR1, sigaction(), sigemptyset()
#include <string.h>             // strlen()
#include <stdlib.h>             // system()
#include <errno.h>             	// errno

// Common headers
#include "socket.h"             // socket_client_connect()
#include "debug.h"          // DEBUG

// Project headers
#include "my_client.h"          // test_servers()










/************************************
 *			DEFINE					*
 ************************************/
#define SERVER_IP   "127.0.0.1"
#define BUF_LEN     128

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)










/************************************
 *			FORWARDS				*
 ************************************/
static void sigusr1_handler 	(int signal);
static void sigint_handler  	(int signal);
static void print_new_msgs  	(t_dynamic_vector *servers);
static void close_all_servers 	(t_dynamic_vector *servers);










/************************************
 *			VARIABLES				*
 ************************************/
static bool client_close = false;










/************************************
 *				MAIN				*
 ************************************/
int main (int argc, char *argv[])
{
	/* Block SIGUSR1 and SIGINT; other threads created by main()
		will inherit a copy of the signal mask. */
	sigset_t set;
	int s;

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGPIPE);
	if ((s = pthread_sigmask(SIG_BLOCK, &set, NULL)) != 0)
		handle_error_en(s, "pthread_sigmask");




	// Uso threads para comunicar cada hijo con el padre

	struct sigaction sigact;
	sigemptyset (&sigact.sa_mask);

	sigact.sa_handler = sigusr1_handler;
	sigaction (SIGUSR1, &sigact, NULL);

	sigact.sa_handler = sigint_handler;
	sigaction (SIGINT, &sigact, NULL);

	t_dynamic_vector servers;

	LOG ("connecting to servers...\n");
	test_servers (&servers);
	create_threads (&servers);

	// Vuelvo a habilitar las interrupciones (solo interrumpe al thread principal)
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	if ((s = pthread_sigmask(SIG_UNBLOCK, &set, NULL)) != 0)
		handle_error_en(s, "pthread_sigmask");

	// while principal
	while (pause(), !client_close)
	{
		print_new_msgs (&servers);

		if (client_close)
		{
			break;
		}
	}

	LOG ("\nCerrando... espere\n");

	close_all_servers (&servers);

	LOG ("\nCerrado correctamente\n");


	return RET_MAIN_OK;
}










static void print_new_msgs (t_dynamic_vector *servers)
{
	int 						i;
	t_dynamic_vector_element *	element;
	t_server *					server;


	for (i = servers->num_elements - 1; i >= 0; i--)
	{
		element = servers->elements + i;
		server  = (t_server *) element->element;

		if (server->thread_read_closed && server->thread_write_closed)
		{
			dynamic_vector_remove (servers, element->key);
		}
		else if (server->recieved)
		{
			server->recieved = false;
			LOG ("%s sent: %s\n", server->ip, server->buffer);
		}
	}

	if (servers->num_elements == 0)
	{
		client_close = true;
	}
}










static void close_all_servers (t_dynamic_vector *servers)
{
	int i;
	t_server *server;

	for (i = servers->num_elements - 1; i >= 0; i--)
	{
		server = (t_server *) (servers->elements[i].element);

		server->thread_closing = true;
		pthread_kill (server->thread_read, 	SIGUSR1); // mando signal para que salga del sleep
		pthread_kill (server->thread_write, SIGUSR1); // mando signal para que salga del sleep

		close (server->sock_fd);
	}

	// Espero que se mueran todos los threads
	while (servers->num_elements)
	{
		print_new_msgs (servers);
	}
}










static void sigusr1_handler (int signal)
{
	LOG ("sigusr1\n");
	// Cuando recibe un dato o cuando mueren los threads de lectura y escritura
	// (No hace nada. Es para que salga del sleep del main)
}










static void sigint_handler (int signal)
{
	LOG ("sigint\n");
	client_close = true;
}