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
	while(1)
	{
		_screen_write_str("Tarea Idle\n");

		_hlt();
	}
}