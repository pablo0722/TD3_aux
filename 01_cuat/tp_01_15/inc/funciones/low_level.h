#ifndef _LOW_LEVEL_H_
#define _LOW_LEVEL_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA 	 					*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KFUNCTION__ void 	_load_tr();
__KFUNCTION__ void 	_load_idt(Idtr idtr);
__KFUNCTION__ void 	_set_cr3(CR3 cr3);
__KFUNCTION__ dword _get_cr3();
__KFUNCTION__ void 	_paging_enable();

__KFUNCTION__ void 	_hlt();


/*** SYSTEM CALLS ***/

__FUNCTION__ void _task_hlt						();
__FUNCTION__ void _task_delay					( dword delay_ms );
__FUNCTION__ dword _read						( void * buffer, dword num_bytes );
__FUNCTION__ void _screen_reset					();
__FUNCTION__ void _screen_set_color 			( byte flags );
__FUNCTION__ byte _screen_get_color 			();
__FUNCTION__ void _screen_write_char 	 		( 		char c, 		byte flags );
__FUNCTION__ void _screen_write_str 			( const char *string );
__FUNCTION__ void _screen_write_str_color 		( const char *string, 	byte flags );
__FUNCTION__ void _screen_write_str_nl 			( const char *string );
__FUNCTION__ void _screen_write_str_nl_color 	( const char *string, 	byte flags );
__FUNCTION__ void _screen_write_byte 			( 		byte b );
__FUNCTION__ void _screen_write_word 			( 		word w );
__FUNCTION__ void _screen_write_dword 			( 		dword dw );
__FUNCTION__ void _screen_write_dword_color 	( 		dword dw, 		byte flags );
__FUNCTION__ void _screen_write_byte_hex 		( 		byte b );
__FUNCTION__ void _screen_write_word_hex 		( 		word w );
__FUNCTION__ void _screen_write_dword_hex 		( 		dword dw );
__FUNCTION__ void _screen_write_dword_hex_color ( 		dword dw, 		byte flags );

#endif // _LOW_LEVEL_H_