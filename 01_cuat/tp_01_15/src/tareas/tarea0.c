/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"
#include "funciones/funciones.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DEFINES LINKER				*
 ************************************************/
extern dword __SHDW_IDLE_TEXT_DST;
extern dword __SHDW_IDLE_TEXT_PHY;
extern dword __SHDW_IDLE_TEXT_ORG;
extern dword __SHDW_IDLE_TEXT_LEN;

extern dword __SHDW_IDLE_BSS_DST;
extern dword __SHDW_IDLE_BSS_PHY;
extern dword __SHDW_IDLE_BSS_LEN;

extern dword __SHDW_IDLE_DATA_DST;
extern dword __SHDW_IDLE_DATA_PHY;
extern dword __SHDW_IDLE_DATA_ORG;
extern dword __SHDW_IDLE_DATA_LEN;

extern dword __STACK_TAREA_NIVEL0_LIN_END;










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					LOCAL FORWARDS				*
 ************************************************/










/************************************************
 * 					TAREA0 				*
 ************************************************/
__TEXT__ void _idle()
{
	asm("mov %%esp, %0"::"r" (&__STACK_TAREA_NIVEL0_LIN_END));

	while(1)
	{
	#ifdef TASK_PRINT
		_screen_write_str("Tarea Idle\n");
	#endif

		_hlt();
	}
}