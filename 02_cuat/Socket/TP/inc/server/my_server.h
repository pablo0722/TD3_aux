#ifndef MY_SERVER_H_
#define MY_SERVER_H_










/************************************
 *          INCLUDES                *
 ************************************/
#include "socket.h"     // IP_LEN
#include "common.h"     // NAME_LEN
#include "basics.h"     // bool









/************************************
 *          DEFINES                 *
 ************************************/
#define MAX_CLIENTS     1000
#define SEND_TIME       1
#define BUFFER_LEN      128










/************************************
 *          STRUCTS                 *
 ************************************/
typedef struct client
{
    int  sock_fd;
    int  client_fd;
    char ip   [IP_LEN];
    char name [NAME_LEN];
    bool close_thread;
    int  send_time;
} t_client;










/************************************
 *          ENUMS                   *
 ************************************/
enum E_RET_MAIN
{
    RET_MAIN_OK,
    RET_MAIN_INSUFICIENT_ARGS,
    RET_MAIN_INVALID_ARGS,
    RET_MAIN_ERROR_SOCKET_OPEN,
    RET_MAIN_ERROR_THREAD_ATTR_INIT,
    RET_MAIN_ERROR_THREAD_ATTR_DETACH,
    RET_MAIN_ERROR_THREAD_CREATE
};










/************************************
 *          FUNCTIONS               *
 ************************************/
void create_thread     (t_client *child_arg);
void server_child_func (t_client *arg);










#endif // MY_SERVER_H_