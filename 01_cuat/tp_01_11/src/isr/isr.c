/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "isr/isr.h"

#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"
#include "funciones/funciones.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					PIC ISR 					*
 ************************************************/
__ISR__ void _isr_tm0()
{
	tick++;
}

__ISR__  void _isr_kb(dword key) 	// KeyBoard isr 1 del PIC 1
{
	_ring_buffer_push(&ring_buffer_keyboard, &key, 1);
}










/************************************************
 * 					EXCEPTION ISR 				*
 ************************************************/
__ISR__ void _isr_de() 	// Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
{

}

__ISR__ void _isr_nmi()
{

}

__ISR__ void _isr_bp()
{

}

__ISR__ void _isr_of()
{

}

__ISR__ void _isr_br()
{

}

__ISR__ void _isr_ud() 	// Invalid Opcode Exception
{

}

__ISR__ void _isr_nm()
{

}

__ISR__ void _isr_df() 	// Doble Fault
{

}

__ISR__ void _isr_ts()
{

}

__ISR__ void _isr_np()
{

}

__ISR__ void _isr_ss()
{

}

__ISR__ void _isr_gp() 	// General Protection
{

}

__ISR__ void _isr_pf() 	// Page Fault
{

}

__ISR__ void _isr_mf()
{

}

__ISR__ void _isr_ac()
{

}

__ISR__ void _isr_mc()
{

}

__ISR__ void _isr_xf()
{

}
