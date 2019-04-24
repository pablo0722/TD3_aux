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
        return RET_SERVER_ALREADY_OPENED;
    }

    /* Creamos el socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)
    {
        return RET_SERVER_ERROR_SOCKET;
    }

    /* Establecemos server_info con la direccion del server */
    server_info.sin_family      = AF_INET;
    server_info.sin_port        = htons(server_port);
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_info.sin_zero), 8);

    if (bind
        (
            sock_fd,(const struct sockaddr *) &server_info, 
            sizeof(struct sockaddr)
        ) == -1)
    {
        return RET_SERVER_ERROR_BIND;
    }

    if (listen(sock_fd, backlog) == -1)
    {
        return RET_SERVER_ERROR_LISTEN;
    }

    server_opened = true;

    return sock_fd;
}










int socket_server_accept (int sock_fd, char *client_ip)
{
    struct sockaddr client_addr;
    socklen_t client_addr_len = sizeof (struct sockaddr);
    
    int client_fd;
    
    while (client_fd = accept( sock_fd, &client_addr, &client_addr_len),
        errno == EINTR);

    if (client_ip)
    {
        while (inet_ntop( AF_INET, &(((struct sockaddr_in *)&client_addr)->sin_addr),
            client_ip, INET_ADDRSTRLEN ),
            errno == EINTR);
    }

    if (client_fd == -1)
    {
        return RET_CLIENT_ERROR_ACCEPT;
    }

    return client_fd;
}










int socket_client_connect(char *server_ip, int server_port)
{
    int	sock_fd;
    struct sockaddr_in server_info;

    // Creamos el socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)
    {
        return RET_CLIENT_ERROR_SOCKET;
    }

    // Establecemos server_info con la direccion del server
    server_info.sin_family      = AF_INET;
    server_info.sin_port        = htons(server_port);
    server_info.sin_addr.s_addr = inet_addr(server_ip);
    bzero(&(server_info.sin_zero), 8);

    // Intentamos conectarnos con el servidor
    if (connect(sock_fd, (struct sockaddr *) &server_info, sizeof(struct sockaddr)) == -1)
    {
        return RET_CLIENT_ERROR_CONNECT;
    }

    return sock_fd;
}