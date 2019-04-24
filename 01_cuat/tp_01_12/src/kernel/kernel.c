/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "funciones/funciones.h"
#include "tareas/tareas.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA						*
 ************************************************/
dword key __BSS__;










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KERNEL__ void _kernel_main()
{
	dword tick_old = tick;

	screen_reset();
	screen_write_str("_kernel_main\n");

	_tarea1_init();

	_kernel_init();

	screen_write_str("_kernel_while\n");

	while(1)
	{
		if(tick < tick_old+2) // cada 100 ms (2*50ms)
		{
			_hlt();
		}
		tick_old = tick;

		key = 0;
		_ring_buffer_pop(&ring_buffer_keyboard, &key, 1);

		if ( key == KEY_ESC)
		{
			screen_reset();
		}

		_tarea1();
	}



	while(1)
	{
		_hlt();
	}
}