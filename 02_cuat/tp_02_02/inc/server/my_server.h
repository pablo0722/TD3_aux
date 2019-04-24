#ifndef MY_SERVER_H_
#define MY_SERVER_H_










/************************************
 *          INCLUDES                *
 ************************************/
#include "socket.h"             // IP_LEN
#include "common.h"             // NAME_LEN
#include "basics.h"             // bool
#include "dynamic_vector.h"     // t_dynamic_vector, dynamic_vector_create(), dynamic_vector_add(), dynamic_vector_remove(), dynamic_vector_free()









/************************************
 *          DEFINES                 *
 ************************************/
#define MAX_CLIENTS     1000
#define SEND_TIME       1
#define BUFFER_LEN      128










/************************************
 *          STRUCTS                 *
 ************************************/
typedef struct
{
    int         sem_id;
    struct tm   date;
    int         value;
} t_buffer;





typedef struct
{
    int                 sock_fd;
    t_dynamic_vector *  clients;
    int                 client_fd;
    char                ip   [IP_LEN];
    char                name [NAME_LEN];
    int                 send_time;
    t_buffer *          buffer_A;
    t_buffer *          buffer_B;
    int *               sem_id_close_all;
    int                 sem_id_write_running;   // Lock al comienzo del thread write. Unlock al final del thread write. Se usa para saber cuando termino el thread para poder eliminar client_t del dynamic_vector

    /*  XXX_thread_close son bool inicializados en false 
        que solamente son seteadas a true cuando se desea
        cerrar el thread, nunca mas son utilizadas.
        Debido a esto NO hay posibilidad que se de algun
        falso positivo/negativo o condición de carrera,
        por lo tanto no es necesario el uso de semaforo
        o mutex.
    */
    bool        read_thread_close;
    bool        write_thread_close;
} t_client;





typedef struct
{
    int                 sock_fd;
    t_dynamic_vector    clients;
    int                 sem_id_close_all;       // Lock en main_thread (sem = 0). unlock por cada thread creado (sem = N). lock al cerrar cada thread (sem = 0). wait_for_locked al cerrar todo.
    t_buffer            buffer_A;
    t_buffer            buffer_B;
    
    /*  XXX_thread_close son bool inicializados en false 
        que solamente son seteadas a true cuando se desea
        cerrar el thread, nunca mas son utilizadas.
        Debido a esto NO hay posibilidad que se de algun
        falso positivo/negativo o condición de carrera,
        por lo tanto no es necesario el uso de semaforo
        o mutex.
    */
    bool                accept_thread_close;
    bool                device_thread_close;
} t_thread_arg;










/************************************
 *          ENUMS                   *
 ************************************/
enum E_RET_MAIN
{
    RET_MAIN_OK,
    RET_MAIN_INSUFICIENT_ARGS,
    RET_MAIN_INVALID_ARGS,
    RET_MAIN_ERROR_SOCKET_OPEN,
    RET_MAIN_ERROR_BLOCK_SIGNAL,
    RET_MAIN_ERROR_THREAD_ATTR_INIT,
    RET_MAIN_ERROR_THREAD_ATTR_DETACH,
    RET_MAIN_ERROR_THREAD_CREATE
};










/************************************
 *          FUNCTIONS               *
 ************************************/
void *thread_read_func(void *arg);
void *thread_write_func(void *arg);










#endif // MY_SERVER_H_