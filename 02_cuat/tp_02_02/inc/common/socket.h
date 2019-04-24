#ifndef SOCKET_H_
#define SOCKET_H_










/************************************
 *          DEFINES                 *
 ************************************/
#define CLIENT_ALIVE_TIMEOUT    20
#define IP_LEN                  20










/************************************
 *          ENUMS                   *
 ************************************/
enum RET_SOCKET_SERVER
{
    RET_SOCKET_SERVER_ALREADY_OPENED = 1,
    RET_SOCKET_SERVER_ERROR_SOCKET,
    RET_SOCKET_SERVER_ERROR_BIND,
    RET_SOCKET_SERVER_ERROR_LISTEN
};

enum RET_SOCKET_ACCEPT
{
    RET_SOCKET_ACCEPT_ERROR_ACCEPT = 1,
    RET_SOCKET_ACCEPT_ERROR_SELECT,
    RET_SOCKET_ACCEPT_TIMEOUT
};

enum RET_SOCKET_CLIENT
{
    RET_SOCKET_CLIENT_ERROR_SOCKET = 1,
    RET_SOCKET_CLIENT_ERROR_CONNECT
};










/************************************
 *          FUNCTIONS               *
 ************************************/
int socket_server_open          (int server_port, int backlog);
int socket_server_accept_timed  (int sock_fd, char *client_ip, int timeout);
int socket_server_accept        (int sock_fd, char *client_ip);
int socket_client_connect       (char *server_ip, int server_port);










#endif // SOCKET_H_