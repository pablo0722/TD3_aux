/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "funciones/funciones.h"
#include "task/tareas.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KERNEL__ void _kernel_main()
{
	dword tick_old = tick;

	_kernel_init();

	_tarea1_init();

	while(1)
	{
		if(tick < tick_old+2) // cada 100 ms (2*50ms)
		{
			_hlt();
		}
		tick_old = tick;

		_tarea1();
	}



	while(1)
	{
		_hlt();
	}
}