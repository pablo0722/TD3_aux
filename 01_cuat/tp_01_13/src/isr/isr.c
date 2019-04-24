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
	_screen_write_str_nl_color("[Exception] Divide Error\n", FG_RED | INTENSE);
}

__ISR__ void _isr_nmi()
{
	_screen_write_str_nl_color("[Exception] nmi\n", FG_RED | INTENSE);
}

__ISR__ void _isr_bp()
{
	_screen_write_str_nl_color("[Exception] bp\n", FG_RED | INTENSE);
}

__ISR__ void _isr_of()
{
	_screen_write_str_nl_color("[Exception] of\n", FG_RED | INTENSE);
}

__ISR__ void _isr_br()
{
	_screen_write_str_nl_color("[Exception] br\n", FG_RED | INTENSE);
}

__ISR__ void _isr_ud() 	// Invalid Opcode Exception
{
	_screen_write_str_nl_color("[Exception] Invalid Opcode\n", FG_RED | INTENSE);
}

__ISR__ void _isr_nm()
{
	_screen_write_str_nl_color("[Exception] nm\n", FG_RED | INTENSE);
}

__ISR__ void _isr_df() 	// Double Fault
{
	_screen_write_str_nl_color("[Exception] Double Fault\n", FG_RED | INTENSE);
}

__ISR__ void _isr_ts()
{
	_screen_write_str_nl_color("[Exception] ts\n", FG_RED | INTENSE);
}

__ISR__ void _isr_np()
{
	_screen_write_str_nl_color("[Exception] np\n", FG_RED | INTENSE);
}

__ISR__ void _isr_ss()
{
	_screen_write_str_nl_color("[Exception] ss\n", FG_RED | INTENSE);
}

__ISR__ void _isr_gp() 	// General Protection
{
	_screen_write_str_nl_color("[Exception] General Protection\n", FG_RED | INTENSE);
	MBP();
}

__ISR__ void _isr_pf(const void *page_dir, dword error_code) 	// Page Fault
{
	/*
		ERROR_CODE:

		31                4                             0
		+-----+-...-+-----+-----+-----+-----+-----+-----+
		|     Reserved    | I/D | RSVD| U/S | W/R |  P  |
		+-----+-...-+-----+-----+-----+-----+-----+-----+

		P: When set, the fault was caused by a protection violation.
		When not set, it was caused by a non-present page.
		W/R: When set, write access caused the fault; otherwise read access.
		U/S: When set, the fault occurred in user mode; otherwise in supervisor mode.
		RSVD: When set, one or more page directory entries contain reserved bits which are set to 1.
		This only applies when the PSE or PAE flags in CR4 are set to 1.
		I/D: When set, the fault was caused by an instruction fetch.
		This only applies when the No-Execute bit is supported and enabled.
	*/

	_screen_write_str_nl_color("[Exception] Page Fault. dir: ", FG_RED | INTENSE);
	_screen_write_dword_hex((dword) page_dir);
	_screen_write_str_color(".\n", FG_RED | INTENSE);

	if(error_code & 0b00001)
	{
		_screen_write_str_color("    Protection violation", FG_RED | INTENSE);
	}
	else
	{
		_screen_write_str_color("    Non-present page", FG_RED | INTENSE);
		//kmalloc(page_dir, PAGE_ATTR_USER | PAGE_ATTR_R, pdt_tarea0, pt_tarea0);
	}

	if(error_code & 0b00010)
	{
		_screen_write_str_color(" at write access", FG_RED | INTENSE);
	}
	else
	{
		_screen_write_str_color(" at read access", FG_RED | INTENSE);
	}

	if(error_code & 0b00100)
	{
		_screen_write_str_color(" in user mode.", FG_RED | INTENSE);
	}
	else
	{
		_screen_write_str_color(" in supervisor mode.", FG_RED | INTENSE);
	}

	if(error_code & 0b01000)
	{
		_screen_write_str_color(" Page directory entry contain reserved bits in 1.", FG_RED | INTENSE);
	}

	if(error_code & 0b10000)
	{
		_screen_write_str_color(" Caused by an instruction fetch.", FG_RED | INTENSE);
	}

	_screen_write_str_color("\n", FG_RED | INTENSE);
}

__ISR__ void _isr_mf()
{
	_screen_write_str_nl_color("[Exception] mf\n", FG_RED | INTENSE);
}

__ISR__ void _isr_ac()
{
	_screen_write_str_nl_color("[Exception] ac\n", FG_RED | INTENSE);
}

__ISR__ void _isr_mc()
{
	_screen_write_str_nl_color("[Exception] mc\n", FG_RED | INTENSE);
}

__ISR__ void _isr_xf()
{
	_screen_write_str_nl_color("[Exception] xf\n", FG_RED | INTENSE);
}
