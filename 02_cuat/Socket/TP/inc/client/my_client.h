#ifndef MY_CLIENT_H_
#define MY_CLIENT_H_










/************************************
 *			INCLUDES				*
 ************************************/
#include "dynamic_vector.h"     // t_dynamic_vector, dynamic_vector_create(), dynamic_vector_add(), dynamic_vector_remove(), dynamic_vector_free()










/************************************
 *			DEFINES					*
 ************************************/
#define BUFFER_LEN 128
#define READ_TIME  1










/************************************
 *			STRUCTS					*
 ************************************/
typedef struct 
{
	int				sock_fd;
	int				poll_time;
	char			buffer [BUFFER_LEN];
	volatile bool	recieved;
	pthread_t		thread_main;
	volatile bool	thread_closing;
	pthread_t		thread_read;
	volatile bool	thread_read_closed;
	pthread_t		thread_write;
	volatile bool	thread_write_closed;
	char			ip[20];
	char			port[10];
} t_server;










/************************************
 *			ENUMS					*
 ************************************/
enum E_RET_MAIN
{
	RET_MAIN_OK,
	RET_MAIN_ERROR_THREAD_ATTR_INIT,
	RET_MAIN_ERROR_THREAD_ATTR_DETACH,
	RET_MAIN_ERROR_THREAD_CREATE,
	RET_MAIN_ERROR_THREAD_SELECT
};










/************************************
 *			FUNCTIONS				*
 ************************************/
void test_servers (t_dynamic_vector *servers);
void create_threads(t_dynamic_vector *servers);










#endif // MY_CLIENT_H_