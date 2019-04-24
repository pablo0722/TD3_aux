/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "tareas/tareas.h"

#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"
#include "funciones/funciones.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DEFINES LINKER				*
 ************************************************/
extern dword __SHDW_TAREA1_TEXT_DST;
extern dword __SHDW_TAREA1_TEXT_PHY;
extern dword __SHDW_TAREA1_TEXT_ORG;
extern dword __SHDW_TAREA1_TEXT_LEN;

extern dword __SHDW_TAREA1_BSS_DST;
extern dword __SHDW_TAREA1_BSS_PHY;
extern dword __SHDW_TAREA1_BSS_LEN;

extern dword __SHDW_TAREA1_DATA_DST;
extern dword __SHDW_TAREA1_DATA_PHY;
extern dword __SHDW_TAREA1_DATA_ORG;
extern dword __SHDW_TAREA1_DATA_LEN;










/************************************************
 * 					LOCAL FORWARDS				*
 ************************************************/
__TEXT__ static void _accum_digits();










/************************************************
 * 					EXCEPTION ISR 				*
 ************************************************/
__TEXT__ void _tarea1()
{
	while(1)
	{
	#ifdef TASK_PRINT
		_screen_write_str("Tarea 1\n");
	#endif

		_accum_digits();
		
		_task_delay(100);
	}
}










__TEXT__ static void _accum_digits()
{
	static 	dword prev_acum __BSS__ = 0;
	static 	dword digito[MAX_RING_BUFFER_SIZE] __BSS__ ALIGN(16);
	static 	dword accum_digits __BSS__ ALIGN(16);
			dword i;
			dword cant_leida;

	accum_digits = 0;

	for(i=0; i<MAX_RING_BUFFER_SIZE; i++)
		digito[i] = 0;

	cant_leida = _read(digito, MAX_RING_BUFFER_SIZE);

	_simd_suma_desborde_dword(digito, &accum_digits, cant_leida);

	if(prev_acum != accum_digits)
	{
		prev_acum = accum_digits;
		_screen_write_str("T1: accum_digits = ");
		_screen_write_dword_hex((dword) accum_digits);

		_screen_write_str("\n");
	}
}