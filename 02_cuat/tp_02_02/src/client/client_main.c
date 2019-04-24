/************************************
 *			INCLUDES				*
 ************************************/
// C headers
#include <stdio.h>              // printf ()
#include <unistd.h>             // read(), write(), close()
#include <signal.h>             // SIGUSR1, sigaction(), sigemptyset()
#include <string.h>             // strlen()
#include <stdlib.h>             // system()
#include <errno.h>             	// errno

// Common headers
#include "common.h"     		// MSG_LIGHT
#include "socket.h"             // socket_client_connect()
#include "debug.h"          	// DEBUG
#include "basics.h"     		// bool, handle_error_en()
#include "IPC.h"     			// xxx_semaphore()

// Project headers
#include "my_client.h"          // test_servers()










/************************************
 *			DEFINE					*
 ************************************/
#define SERVER_IP   "127.0.0.1"
#define BUF_LEN     128










/************************************
 *			FORWARDS				*
 ************************************/
static void close_all_servers 	(t_dynamic_vector *servers);










/************************************
 *				MAIN				*
 ************************************/
int main (int argc, char *argv[])
{
	/* Block SIGPIPE and SIGINT; other threads created by main()
		will inherit a copy of the signal mask.
		SIGPIPE: Cuando intenta escribir al servidor pero ya cerro la conexion*/
	sigset_t set;
	int s, signal;

	bool client_close = false;

	LOG ("Blocking signals... ");
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGPIPE);
	if ((s = pthread_sigmask(SIG_BLOCK, &set, NULL)) != 0)
		handle_error_en(s, "pthread_sigmask");
	LOG ("OK\n");




	// Uso threads para comunicar cada hijo con el padre

	t_dynamic_vector servers;

	test_servers (&servers);

	create_threads (&servers);


	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGINT);


	// while principal
	while (sigwait(&set, &signal), !client_close && servers.num_elements > 0)
	{
		if (signal == SIGINT)
		{
			client_close = true;
		}

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










static void close_all_servers (t_dynamic_vector *servers)
{
	int i;
	t_server *server;

	for (i = servers->num_elements - 1; i >= 0; i--)
	{
		server = (t_server *) (servers->elements[i].element);

		server->thread_closing = true;

		close (server->sock_fd);
	}
}