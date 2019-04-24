/************************************
 *          INCLUDES                *
 ************************************/
// Main header
#include "socket.h"

// C headers
#include <sys/socket.h> // AF_INET, SOCK_STREAM
#include <stdio.h>      // NULL
#include <netinet/in.h> // struct sockaddr_in
#include <netdb.h>      // struct hostent
#include <arpa/inet.h>  // inet_addr()
#include <strings.h>    // bzero()
#include <errno.h>      // errno

// Common headers
#include "basics.h"










/************************************
 *          VARIABLES               *
 ************************************/
static struct sockaddr_in server_info;










/************************************
 *          FUNCTIONS               *
 ************************************/
int socket_server_open(int server_port, int backlog)
{
    static bool server_opened = false;
    int	sock_fd;

    if (server_opened)
    {
        return -RET_SOCKET_SERVER_ALREADY_OPENED;
    }

    /* Creamos el socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)
    {
        return -RET_SOCKET_SERVER_ERROR_SOCKET;
    }

    /* Establecemos server_info con la direccion del server */
    server_info.sin_family      = AF_INET;
    server_info.sin_port        = htons(server_port);
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_info.sin_zero), 8);

    if  (bind
            (
                sock_fd,(const struct sockaddr *) &server_info, 
                sizeof(struct sockaddr)
            ) == -1
        )
    {
        return -RET_SOCKET_SERVER_ERROR_BIND;
    }

    if (listen(sock_fd, backlog) == -1)
    {
        return -RET_SOCKET_SERVER_ERROR_LISTEN;
    }

    server_opened = true;
    
    return sock_fd;
}










int socket_server_accept_timed (int sock_fd, char *client_ip, int timeout)
{
    int return_value;

    struct sockaddr client_addr;
    socklen_t client_addr_len = sizeof (struct sockaddr);
    
    int client_fd;

    fd_set rfds;
    struct timeval tv;
    struct timeval *actual_tv;
    int select_ret_val;


    do
    {
        FD_ZERO(&rfds);
        FD_SET(sock_fd, &rfds);

        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        actual_tv = &tv;

        if (timeout < 0)
        {
            // espera infinito
            actual_tv = NULL;
        }

        select_ret_val = select (sock_fd + 1, &rfds, NULL, NULL, actual_tv);
    } while (errno == EINTR);

    if (select_ret_val == -1)
    {
        perror("Error select()");

        return_value = -RET_SOCKET_ACCEPT_ERROR_SELECT;
    }
    else if (select_ret_val)
    {
        while (client_fd = accept( sock_fd, &client_addr, &client_addr_len),
            errno == EINTR && client_fd == -1);

        if (client_ip)
        {
            while (inet_ntop( AF_INET, &(((struct sockaddr_in *)&client_addr)->sin_addr),
                client_ip, INET_ADDRSTRLEN ),
                errno == EINTR);
        }

        if (client_fd == -1)
        {
            return_value = -RET_SOCKET_ACCEPT_ERROR_ACCEPT;
        }
        else
        {
            return_value = client_fd;
        }
    }
    else
    {
        // timeout

        return_value = -RET_SOCKET_ACCEPT_TIMEOUT;
    }

    return return_value;
}










int socket_server_accept (int sock_fd, char *client_ip)
{
    int return_value;

    struct sockaddr client_addr;
    socklen_t client_addr_len = sizeof (struct sockaddr);

    int client_fd;


printf("ANTES accept\n");
    while (client_fd = accept( sock_fd, &client_addr, &client_addr_len),
        errno == EINTR && client_fd == -1) printf("accept\n");
printf("DESPUES accept\n");

    if (client_ip)
    {
        while (inet_ntop( AF_INET, &(((struct sockaddr_in *)&client_addr)->sin_addr),
            client_ip, INET_ADDRSTRLEN ),
            errno == EINTR);
    }

    if (client_fd == -1)
    {
        return_value = -RET_SOCKET_ACCEPT_ERROR_ACCEPT;
    }
    else
    {
        return_value = client_fd;
    }


    return return_value;
}










int socket_client_connect(char *server_ip, int server_port)
{
    int	sock_fd;
    struct sockaddr_in server_info;

    // Creamos el socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)
    {
        return -RET_SOCKET_CLIENT_ERROR_SOCKET;
    }

    // Establecemos server_info con la direccion del server
    server_info.sin_family      = AF_INET;
    server_info.sin_port        = htons(server_port);
    server_info.sin_addr.s_addr = inet_addr(server_ip);
    bzero(&(server_info.sin_zero), 8);

    // Intentamos conectarnos con el servidor
    if (connect(sock_fd, (struct sockaddr *) &server_info, sizeof(struct sockaddr)) == -1)
    {
        return -RET_SOCKET_CLIENT_ERROR_CONNECT;
    }

    return sock_fd;
}