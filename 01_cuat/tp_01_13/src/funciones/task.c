/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "funciones/task.h"

#include "kernel/kernel.h"
#include "funciones/low_level.h"










/************************************************
 * 					ESTRUCTURAS					*
 ************************************************/
typedef struct PACKED
{
	dword cr3;
	dword busy;
} task_status_t;









/************************************************
 * 					DATA						*
 ************************************************/
extern task_status_t	tasks_vector 		__SYS_TABLES__;
extern dword 			tasks_vector_idx 	__SYS_TABLES__;










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _task_delay ( int delay_ms )
{
	(&tasks_vector)[ (int) tasks_vector_idx ].busy = ( delay_ms / 10 ) + 1;
	
	_hlt();
}