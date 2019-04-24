#ifndef _SCREEN_H_
#define _SCREEN_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/
#define FRAME_BUFFER 			0xB8000

// Colores de fuente
#define	FG_RED		0x04
#define	FG_GREEN	0x02
#define	FG_BLUE		0x01

// Colores de fondo
#define	BG_RED		0x40
#define	BG_GREEN	0x20
#define	BG_BLUE		0x10

// Parpadeo
#define	BLINK		0x80

// Intensidad
#define	INTENSE		0x08










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KFUNCTION__ void _k_screen_reset();
__KFUNCTION__ void _k_screen_set_color( byte flags );
__KFUNCTION__ byte _k_screen_get_color();
__KFUNCTION__ void _k_screen_write_char( char c );
__KFUNCTION__ void _k_screen_write_char_color( char c, byte flags );
__KFUNCTION__ void _k_screen_write_str( const char *string );
__KFUNCTION__ void _k_screen_write_str_color( const char *string, byte flags );
__KFUNCTION__ void _k_screen_write_str_nl( const char *string );
__KFUNCTION__ void _k_screen_write_str_nl_color( const char *string, byte flags );
__KFUNCTION__ void _k_screen_write_byte( byte b );
__KFUNCTION__ void _k_screen_write_word( word w );
__KFUNCTION__ void _k_screen_write_dword( dword dw );
__KFUNCTION__ void _k_screen_write_dword_color( dword dw, byte flags );
__KFUNCTION__ void _k_screen_write_byte_hex( byte b );
__KFUNCTION__ void _k_screen_write_word_hex( word w );
__KFUNCTION__ void _k_screen_write_dword_hex( dword dw );
__KFUNCTION__ void _k_screen_write_dword_hex_color( dword dw, byte flags );










#endif // _SCREEN_H_