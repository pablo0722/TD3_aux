/**
    @file       main.c
    @author     Koremblum Nicolas Mariano (n.m.koremblum@ieee.org)
    @date       28/08/2018 (dd/mm/yyyy)
    @version    1.0
    @brief      ?

        Compilar con: gcc -o prueba -pthread main.c -Wall

    @todo       ?
**/

/* ------------------------------------------------------------ ;
;                                                               ;
;                           INCLUDE's                           ;
;                                                               ;
; ------------------------------------------------------------ */

#include	<stddef.h>
#include	<stdio.h>

#include	<stdlib.h>

#include	<unistd.h>

#include	<strings.h>

#include	<signal.h>

#include	<sys/select.h>

#include	<sys/types.h>

#include	<errno.h>

#include	<sys/socket.h>

#include	<sys/wait.h>

#include	<netinet/in.h>

#include	<arpa/inet.h>

#include	<netdb.h>

#include    <pthread.h>

/* ------------------------------------------------------------ ;
;                                                               ;
;                           DEFINE's                            ;
;                                                               ;
; ------------------------------------------------------------ */

#define		TCP_SERVER_PORT			3490 // > 1024

#define		KEYBOARD_FD				0

#define		USEC_SELECT_TIMEOUT		0

#define		SEC_SELECT_TIMEOUT		1

#define     IS_ERROR(x)             ( (0 > x)  ? 1:0 )

#define		IS_TIMEOUT(x)           ( (0 == x) ? 1:0 )

#define 	IS_CHILD_PROCCESS(x)	( (0 == x) ? 1:0 )

/* ------------------------------------------------------------ ;
;                                                               ;
;                             CODE                              ;
;                                                               ;
; ------------------------------------------------------------ */

void * zombie_hunter(void * frula)
{
    while( waitpid(-1, NULL, WNOHANG) );

    return NULL;
}

void sigcld_handler(int signalParameter)
{
    pthread_t threadId;
    
    pthread_create(&threadId, NULL, zombie_hunter, NULL);
}

int tcp_openConnection( int tcpServerFD, struct sockaddr_in *tcpServerAddr )
{
    // Implementar
    return 0;
}

int tcp_connectionClient(int fdServer, struct sockaddr_in * tcpClientAddr)
{
    // Implementar
    return 0;
}

int child_client_function(int fdClient, struct sockaddr_in * tcpClientAddr)
{
    // Implementar
    return 0;
}

int main(int argc, char *argv[])
{
    /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        VARIABLES DECLARATION
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    //  Select's
    int     selRetVal,  maxFD;
    fd_set  fdsMaster,  fdsBackup;
    struct  timeval     timeout;

    //  Socket's
    int     fdServer,   fdClient;
    struct  sockaddr_in tcpServerAddr, tcpClientAddr;

    //  Signal's
	struct  sigaction   strSigaction;

    //  Others
    pid_t   childPid = -1;

    /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        VARIABLES INITIALIZATION
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    fdServer = -1;
    fdClient = -1;
    
    /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            SIGNALS HANDLING
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    strSigaction.sa_handler = sigcld_handler;   // Func ptr
	strSigaction.sa_flags = SA_RESTART;       // Flag
	sigemptyset(&strSigaction.sa_mask);         // ?

    if( IS_ERROR( sigaction(SIGCLD,&strSigaction,NULL) ) )
	{
		perror("\nSigaction Error?:");
		exit(1);
	}

    /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        SOCKET OPENING
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    fdServer = tcp_openConnection(  TCP_SERVER_PORT, \
                                    &tcpServerAddr );

	if(	IS_ERROR(fdServer)	)
	{
		printf("\nDebug(Servidor): Error TCP\n");

		return -1;
	}

    /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            SELECT
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    //  Select's Initialization
    FD_ZERO( &fdsMaster );
	//  File Descriptors to monitorate
	FD_SET( KEYBOARD_FD, &fdsMaster );	//  Keyboard is set
	FD_SET( fdServer, &fdsMaster );	//  TCP Socket is set

    // Select needs as a parameter the biggest file descriptor
	maxFD = fdServer;

	//  Timeout' Setting
	timeout.tv_sec 	= SEC_SELECT_TIMEOUT;
	timeout.tv_usec = USEC_SELECT_TIMEOUT;

    fdsBackup = fdsMaster;

    while(1)
    {
        fdsMaster = fdsBackup;

        selRetVal = select(maxFD,&fdsMaster,NULL,NULL,&timeout);

        if( !IS_ERROR(selRetVal) )
        {
			if( FD_ISSET( KEYBOARD_FD , &fdsMaster ) )
			{
            }

            else if( FD_ISSET( fdServer , &fdsMaster ) )
			{
				//Abro el socket TCP para atender a un cliente
				fdClient = tcp_connectionClient(    fdServer,\
                                                    &tcpClientAddr );

				if( IS_ERROR(fdClient) )
				{
					printf("\nError TCP\n");

					exit (1);
				}

                childPid = 0;
                childPid = fork();

				if( IS_ERROR( childPid ) )
                {
                    perror("\nFork Error?: ");
                    exit(1);
                }
                
                // Child Process' function call
				if( IS_CHILD_PROCCESS(childPid) )
                {
                    child_client_function(  fdClient,\
                                            &tcpClientAddr);
                }
            }

            // Timeout's Handling
            else if( IS_TIMEOUT( selRetVal ) )
			{
                // Timeout count's reset
                timeout.tv_sec 	= SEC_SELECT_TIMEOUT;
				timeout.tv_usec = USEC_SELECT_TIMEOUT;
            }

            // ?
            else
            {
                // Timeout count's reset
                timeout.tv_sec 	= SEC_SELECT_TIMEOUT;
				timeout.tv_usec = USEC_SELECT_TIMEOUT;
            }
        }
        
        else
        {
            perror("\nSelect Error?: ");
        }

    }

    /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        RESOURCES DEPOSITION
                            free/close
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    close(fdServer);

    return 0;
}