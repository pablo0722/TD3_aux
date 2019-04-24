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
enum SYS_CALL_FUNCS
{
	SC_TASK_HALT,
	SC_TASK_DELAY,
	SC_READ,
	SC_SCREEN_RESET,
	SC_SCREEN_SET_COLOR,
	SC_SCREEN_GET_COLOR,
	SC_SCREEN_WRITE_CHAR,
	SC_SCREEN_WRITE_CHAR_COLOR,
	SC_SCREEN_WRITE_STR,
	SC_SCREEN_WRITE_STR_COLOR,
	SC_SCREEN_WRITE_NL,
	SC_SCREEN_WRITE_NL_COLOR,
	SC_SCREEN_WRITE_BYTE,
	SC_SCREEN_WRITE_WORD,
	SC_SCREEN_WRITE_DWORD,
	SC_SCREEN_WRITE_DWORD_COLOR,
	SC_SCREEN_WRITE_BYTE_HEX,
	SC_SCREEN_WRITE_WORD_HEX,
	SC_SCREEN_WRITE_DWORD_HEX,
	SC_SCREEN_WRITE_DWORD_HEX_COLOR,
};









/************************************************
 * 					PIC ISR 					*
 ************************************************/
__ISR__ static void _get_hexa_keys(dword key)
{
	static 	dword 	i 		__MY_BSS__ 	= 0;
	static 	dword 	digito 	__MY_BSS__ 	= 0;
			dword 	valor 				= -1;
			dword 	caracter 			= 0;


	if(key != 0)
	{
		switch(key)
		{
			case KEY_0:
			case KEY_0_PAD:
				valor 		= 0;
				caracter 	= '0';
				break;

			case KEY_1:
			case KEY_1_PAD:
				valor 		= 1;
				caracter 	= '1';
				break;

			case KEY_2:
			case KEY_2_PAD:
				valor 		= 2;
				caracter 	= '2';
				break;

			case KEY_3:
			case KEY_3_PAD:
				valor 		= 3;
				caracter 	= '3';
				break;

			case KEY_4:
			case KEY_4_PAD:
				valor 		= 4;
				caracter 	= '4';
				break;

			case KEY_5:
			case KEY_5_PAD:
				valor 		= 5;
				caracter 	= '5';
				break;

			case KEY_6:
			case KEY_6_PAD:
				valor 		= 6;
				caracter 	= '6';
				break;

			case KEY_7:
			case KEY_7_PAD:
				valor 		= 7;
				caracter 	= '7';
				break;

			case KEY_8:
			case KEY_8_PAD:
				valor 		= 8;
				caracter 	= '8';
				break;

			case KEY_9:
			case KEY_9_PAD:
				valor 		= 9;
				caracter 	= '9';
				break;

			case KEY_A:
				valor 		= 10;
				caracter 	= 'A';
				break;

			case KEY_B:
				valor 		= 11;
				caracter 	= 'B';
				break;

			case KEY_C:
				valor 		= 12;
				caracter 	= 'C';
				break;

			case KEY_D:
				valor 		= 13;
				caracter 	= 'D';
				break;

			case KEY_E:
				valor 		= 14;
				caracter 	= 'E';
				break;

			case KEY_F:
				valor 		= 15;
				caracter 	= 'F';
				break;
		}

		if(valor != -1)
		{
			if (i < 8)
			{
				_k_screen_write_str_nl 	( "tecla: " );
				_k_screen_write_char 	( caracter );
				_k_screen_write_str 	( "\n" );

				digito = (digito << 4) | valor;
				i++;
				valor = -1;
			}
			else
			{
				_k_screen_write_str_color ( "Presione ENTER\n", FG_RED | FG_GREEN | INTENSE );
			}
		}

		if (key == KEY_ENTER)
		{
			_k_screen_write_str ( "tecla: ENTER\n" );

			if( digito != 0 )
			{
				if(_ring_buffer_push(&ring_buffer_tabla_digitos, &digito, 1) == 0)
				{
					/* No hay mas lugar, borro uno (popeo de la tabla en una
					 * variable dummy) para hacer lugar, y vuelvo a pushear
					 */

					_ring_buffer_pop(&ring_buffer_tabla_digitos, NULL, 1);
					_ring_buffer_push(&ring_buffer_tabla_digitos, &digito, 1);
				}
				
				digito = 0;
			}

			i = 0;
		}
	}
}









/************************************************
 * 					PIC ISR 					*
 ************************************************/
__ISR__ void _isr_tm0()
{
	// Esta funcion no es utilizada en este TP

	tick++;
}



__ISR__  void _isr_kb(dword key) 	// KeyBoard isr 1 del PIC 1
{
	/* Deberia pushear la tecla en un buffer interno (ring_buffer_keyboard)
	 * y luego, a traves de una API, popear del buffer interno y pushear
	 * al buffer de la tarea actualmente activa (tarea que "tiene el foco").
	 * Ahora se realiza todo el proceso de la tecla dentro de la interrupcion
	 * para simplificar.
	 */

	_get_hexa_keys(key);

	// La tecla L limpia la pantalla
	if ( key == KEY_L )
	{
		_screen_reset();
	}

	// La tecla ESC reinicia la tabla de digitos
	if ( key == KEY_ESC )
	{
		_ring_buffer_init(&ring_buffer_tabla_digitos);
	}
}



__ISR__ dword _isr_system_call(dword func, dword arg1, dword arg2)
{
	dword ret_value = 0;

	switch(func)
	{
		case SC_TASK_HALT:
			_hlt();
		break;

		case SC_TASK_DELAY:
			_k_task_delay(arg1);
		break;

		case SC_READ:
			ret_value = _k_read( (void *) arg1, arg2);
		break;

		case SC_SCREEN_RESET:
			_k_screen_reset();
		break;

		case SC_SCREEN_SET_COLOR:
			_k_screen_set_color( arg1 );
		break;

		case SC_SCREEN_GET_COLOR:
			ret_value = _k_screen_get_color();
		break;

		case SC_SCREEN_WRITE_CHAR:
			_k_screen_write_char( arg1 );
		break;

		case SC_SCREEN_WRITE_CHAR_COLOR:
			_k_screen_write_char_color( arg1, arg2 );
		break;

		case SC_SCREEN_WRITE_STR:
			_k_screen_write_str( (const char *) arg1 );
		break;

		case SC_SCREEN_WRITE_STR_COLOR:
			_k_screen_write_str_color( (const char *) arg1, arg2 );
		break;

		case SC_SCREEN_WRITE_NL:
			_k_screen_write_str_nl( (const char *) arg1 );
		break;

		case SC_SCREEN_WRITE_NL_COLOR:
			_k_screen_write_str_nl_color( (const char *) arg1, arg2 );
		break;

		case SC_SCREEN_WRITE_BYTE:
			_k_screen_write_byte( arg1 );
		break;

		case SC_SCREEN_WRITE_WORD:
			_k_screen_write_word( arg1 );
		break;

		case SC_SCREEN_WRITE_DWORD:
			_k_screen_write_dword( arg1 );
		break;

		case SC_SCREEN_WRITE_DWORD_COLOR:
			_k_screen_write_dword_color( arg1, arg2 );
		break;

		case SC_SCREEN_WRITE_BYTE_HEX:
			_k_screen_write_byte_hex( arg1 );
		break;

		case SC_SCREEN_WRITE_WORD_HEX:
			_k_screen_write_word_hex( arg1 );
		break;

		case SC_SCREEN_WRITE_DWORD_HEX:
			_k_screen_write_dword_hex( arg1 );
		break;

		case SC_SCREEN_WRITE_DWORD_HEX_COLOR:
			_k_screen_write_dword_hex_color( arg1, arg2 );
		break;
	}

	return ret_value;
}










/************************************************
 * 					EXCEPTION ISR 				*
 ************************************************/
__ISR__ void _isr_de() 	// Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
{
	_k_screen_write_str_nl_color("[Exception] Divide Error\n", FG_RED | INTENSE);
}

__ISR__ void _isr_nmi()
{
	_k_screen_write_str_nl_color("[Exception] nmi\n", FG_RED | INTENSE);
}

__ISR__ void _isr_bp()
{
	_k_screen_write_str_nl_color("[Exception] bp\n", FG_RED | INTENSE);
}

__ISR__ void _isr_of()
{
	_k_screen_write_str_nl_color("[Exception] of\n", FG_RED | INTENSE);
}

__ISR__ void _isr_br()
{
	_k_screen_write_str_nl_color("[Exception] br\n", FG_RED | INTENSE);
}

__ISR__ void _isr_ud() 	// Invalid Opcode Exception
{
	_k_screen_write_str_nl_color("[Exception] Invalid Opcode\n", FG_RED | INTENSE);
}

__ISR__ void _isr_nm()
{
	_k_screen_write_str_nl_color("[Exception] nm\n", FG_RED | INTENSE);
}

__ISR__ void _isr_df() 	// Double Fault
{
	_k_screen_write_str_nl_color("[Exception] Double Fault\n", FG_RED | INTENSE);
}

__ISR__ void _isr_ts()
{
	_k_screen_write_str_nl_color("[Exception] ts\n", FG_RED | INTENSE);
}

__ISR__ void _isr_np()
{
	_k_screen_write_str_nl_color("[Exception] np\n", FG_RED | INTENSE);
}

__ISR__ void _isr_ss()
{
	_k_screen_write_str_nl_color("[Exception] ss\n", FG_RED | INTENSE);
}

__ISR__ void _isr_gp() 	// General Protection
{
	_k_screen_write_str_nl_color("[Exception] General Protection\n", FG_RED | INTENSE);
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

	_k_screen_write_str_nl_color("[Exception] Page Fault. dir: ", FG_RED | INTENSE);
	_k_screen_write_dword_hex((dword) page_dir);
	_k_screen_write_str_color(".\n", FG_RED | INTENSE);

	if(error_code & 0b00001)
	{
		_k_screen_write_str_color("    Protection violation", FG_RED | INTENSE);
	}
	else
	{
		_k_screen_write_str_color("    Non-present page", FG_RED | INTENSE);
		//kmalloc(page_dir, PAGE_ATTR_USER | PAGE_ATTR_R, pdt_tarea0, pt_tarea0);
	}

	if(error_code & 0b00010)
	{
		_k_screen_write_str_color(" at write access", FG_RED | INTENSE);
	}
	else
	{
		_k_screen_write_str_color(" at read access", FG_RED | INTENSE);
	}

	if(error_code & 0b00100)
	{
		_k_screen_write_str_color(" in user mode.", FG_RED | INTENSE);
	}
	else
	{
		_k_screen_write_str_color(" in supervisor mode.", FG_RED | INTENSE);
	}

	if(error_code & 0b01000)
	{
		_k_screen_write_str_color(" Page directory entry contain reserved bits in 1.", FG_RED | INTENSE);
	}

	if(error_code & 0b10000)
	{
		_k_screen_write_str_color(" Caused by an instruction fetch.", FG_RED | INTENSE);
	}

	_k_screen_write_str_color("\n", FG_RED | INTENSE);
}

__ISR__ void _isr_mf()
{
	_k_screen_write_str_nl_color("[Exception] mf\n", FG_RED | INTENSE);
}

__ISR__ void _isr_ac()
{
	_k_screen_write_str_nl_color("[Exception] ac\n", FG_RED | INTENSE);
}

__ISR__ void _isr_mc()
{
	_k_screen_write_str_nl_color("[Exception] mc\n", FG_RED | INTENSE);
}

__ISR__ void _isr_xf()
{
	_k_screen_write_str_nl_color("[Exception] xf\n", FG_RED | INTENSE);
}
