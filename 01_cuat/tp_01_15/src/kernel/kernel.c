/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "funciones/funciones.h"
#include "tareas/tareas.h"










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KERNEL__ void _kernel_main()
{
	_kernel_init();

	_k_screen_write_str("_kernel_OS\n");

	_load_idt(idtr);

	_idle(); // Tarea 0
}