/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "funciones/screen.h"

#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"
#include "funciones/keyboard.h"










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KFUNCTION__ dword _k_read( void * buffer, dword num_bytes)
{
	if  ( ( ( _pagine_get_attr ( buffer ) 											& (PAGE_ATTR_USER | PAGE_ATTR_KERNEL) ) == PAGE_ATTR_USER ) && 
		  ( ( _pagine_get_attr ( buffer + ( num_bytes - 1 ) * sizeof(dword) + 3 ) 	& (PAGE_ATTR_USER | PAGE_ATTR_KERNEL) ) == PAGE_ATTR_USER ) )
	{
		asm("hlt"::);
		return _ring_buffer_peek ( &ring_buffer_tabla_digitos, buffer, num_bytes );
	}
	else
	{
		_k_screen_write_str_nl_color 	( "[read] No tiene permisos para leer direccion ", FG_RED | INTENSE);
		_k_screen_write_dword_hex_color ( (dword) buffer, FG_RED | INTENSE );
		_k_screen_write_str 			( "\n" );

		return 0;
	}
}