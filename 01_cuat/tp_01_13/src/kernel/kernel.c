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
RingBuffer 	ring_buffer_tabla_digitos 	__TABLA_DIGITOS__;










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KERNEL__ void _kernel_main()
{
	_screen_reset();
	_screen_write_str("_kernel_main\n");

	_kernel_init();

	_screen_write_str("_kernel_OS\n");

	_load_idt(idtr);

	_idle();
}