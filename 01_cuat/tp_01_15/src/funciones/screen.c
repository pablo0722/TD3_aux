/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "funciones/screen.h"

#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/
#define	MAX_COLUMNAS_PANTALLA	80
#define	MAX_FILAS_PANTALLA		25
#define TAM_VIDEO_CARACTERES    (MAX_FILAS_PANTALLA * MAX_COLUMNAS_PANTALLA)
#define TAM_VIDEO_BYTES			(2 * TAM_VIDEO_CARACTERES)









/************************************************
 * 					DATA						*
 ************************************************/
volatile static char * const	video_buffer 	__RODATA__ 	= (volatile char*)FRAME_BUFFER;
volatile static byte 			video_col 		__BSS__ 	= 0;
volatile static byte 			video_row 		__BSS__ 	= 0;
volatile static byte 			video_flags 	__BSS__ 	= 0;










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KFUNCTION__ static void _k_screen_cursor( byte flags)
{
	int idx = (video_row*MAX_COLUMNAS_PANTALLA + video_col)*2;

	video_buffer[idx] 	= '|';
	video_buffer[idx+1] = video_flags | BLINK;
}










__KFUNCTION__ void _k_screen_reset()
{
	int idx = 0;

	for(idx=0; idx<TAM_VIDEO_BYTES; idx++)
	{
		video_buffer[idx++] = 0;
	}

	_k_screen_set_color(FG_RED | FG_GREEN | FG_BLUE | INTENSE);

	video_col = 0;
	video_row = 0;

	_k_screen_cursor( video_flags);
}










__KFUNCTION__ void _k_screen_set_color( byte flags )
{
	video_flags = flags;
	_k_screen_cursor( flags );
}










__KFUNCTION__ byte _k_screen_get_color()
{
	return video_flags;
}










__KFUNCTION__ void _k_screen_write_char_color( char c, byte flags )
{
	int idx = (video_row*MAX_COLUMNAS_PANTALLA + video_col)*2;

	if(c == '\n')
	{
		video_buffer[idx++] = 0;
		video_buffer[idx] 	= 0;
		video_col 	= 0;
		video_row++;
	}
	else if( (video_col < MAX_COLUMNAS_PANTALLA) && (video_row < MAX_FILAS_PANTALLA) )
	{
		video_buffer[idx++] = c;
		video_buffer[idx] 	= flags;
		video_col++;
	}
	if(video_col >= MAX_COLUMNAS_PANTALLA)
	{
		video_col = 0;
		video_row++;
	}
	if(video_row >= MAX_FILAS_PANTALLA)
	{
		video_col = 0;
		video_row = 0;
	}

	_k_screen_cursor( flags );
}










__KFUNCTION__ void _k_screen_write_char( char c )
{
	_k_screen_write_char_color( c, video_flags );
}










__KFUNCTION__ void _k_screen_write_str( const char *string )
{
	while( *string != 0 )
	{
		_k_screen_write_char_color( *string++, video_flags );
	}
}










__KFUNCTION__ void _k_screen_write_str_color( const char *string, byte flags )
{
	while( *string != 0 )
	{
		_k_screen_write_char_color( *string++, flags );
	}
}










__KFUNCTION__ void _k_screen_write_str_nl( const char *string )
{
	int idx;

	if(video_col != 0)
	{
		idx = (video_row*MAX_COLUMNAS_PANTALLA + video_col)*2;
		video_buffer[idx++] = 0;
		video_buffer[idx] 	= 0;
		video_col 	= 0;
		video_row++;
	}

	_k_screen_write_str( string );
}










__KFUNCTION__ void _k_screen_write_str_nl_color( const char *string, byte flags )
{
	int idx;

	if(video_col != 0)
	{
		idx = (video_row*MAX_COLUMNAS_PANTALLA + video_col)*2;
		video_buffer[idx++] = 0;
		video_buffer[idx] 	= 0;
		video_col 	= 0;
		video_row++;
	}

	_k_screen_write_str_color( string, flags );
}










__KFUNCTION__ void _k_screen_write_byte( byte b )
{
	byte i;
	bool is_cero = 1;

	for(i = 100; i >= 1; i /= 10)
	{
		if(b / i != 0)
		{
			is_cero = 0;
			_k_screen_write_char_color( ((b / i) % 10) + '0', video_flags );
		}
	}

	if(is_cero)
	{
		_k_screen_write_char_color( '0', video_flags );
	}
}










__KFUNCTION__ void _k_screen_write_word( word w )
{
	word i;
	bool is_cero = 1;

	for(i = 10000; i >= 1; i /= 10)
	{
		if(w / i != 0)
		{
			is_cero = 0;
			_k_screen_write_char_color( ((w / i) % 10) + '0', video_flags );
		}
	}

	if(is_cero)
	{
		_k_screen_write_char_color( '0', video_flags );
	}
}










__KFUNCTION__ void _k_screen_write_dword_color( dword dw, byte flags )
{
	dword i;
	bool is_cero = 1;

	for(i = 1000000000; i >= 1; i /= 10)
	{
		if(dw / i != 0)
		{
			is_cero = 0;
			_k_screen_write_char_color( ((dw / i) % 10) + '0', flags );
		}
	}

	if(is_cero)
	{
		_k_screen_write_char_color( '0', flags );
	}
}










__KFUNCTION__ void _k_screen_write_dword( dword dw )
{
	_k_screen_write_dword_color( dw, video_flags );
}










__KFUNCTION__ void _k_screen_write_byte_hex( byte b )
{
	byte i;
	bool is_cero = 1;
	byte hex;
	char character;

	_k_screen_write_char_color( '0', video_flags );
	_k_screen_write_char_color( 'x', video_flags );

	for(i = 0x10; i >= 1; i /= 0x10)
	{
		if(b / i != 0)
		{
			is_cero = 0;
			hex = ((b / i) % 0x10);

			switch(hex)
			{
				case 10:
					character = 'A';
					break;
				case 11:
					character = 'B';
					break;
				case 12:
					character = 'C';
					break;
				case 13:
					character = 'D';
					break;
				case 14:
					character = 'E';
					break;
				case 15:
					character = 'F';
					break;
				default:
					character = hex + '0';
					break;
			}

			_k_screen_write_char_color( character, video_flags );
		}
	}

	if(is_cero)
	{
		_k_screen_write_char_color( '0', video_flags );
	}
}










__KFUNCTION__ void _k_screen_write_word_hex( word w )
{
	word i;
	bool is_cero = 1;
	byte hex;
	char character;

	_k_screen_write_char_color( '0', video_flags );
	_k_screen_write_char_color( 'x', video_flags );

	for(i = 0x1000; i >= 1; i /= 0x10)
	{
		if(w / i != 0)
		{
			is_cero = 0;
			hex = ((w / i) % 0x10);

			switch(hex)
			{
				case 10:
					character = 'A';
					break;
				case 11:
					character = 'B';
					break;
				case 12:
					character = 'C';
					break;
				case 13:
					character = 'D';
					break;
				case 14:
					character = 'E';
					break;
				case 15:
					character = 'F';
					break;
				default:
					character = hex + '0';
					break;
			}

			_k_screen_write_char_color( character, video_flags );
		}
	}

	if(is_cero)
	{
		_k_screen_write_char_color( '0', video_flags );
	}
}










__KFUNCTION__ void _k_screen_write_dword_hex_color( dword dw, byte flags )
{
	dword i;
	bool is_cero = 1;
	byte hex;
	char character;

	_k_screen_write_char_color( '0', flags );
	_k_screen_write_char_color( 'x', flags );

	for(i = 0x10000000; i >= 1; i /= 0x10)
	{
		if(dw / i != 0)
		{
			is_cero = 0;
			hex = ((dw / i) % 0x10);

			switch(hex)
			{
				case 10:
					character = 'A';
					break;
				case 11:
					character = 'B';
					break;
				case 12:
					character = 'C';
					break;
				case 13:
					character = 'D';
					break;
				case 14:
					character = 'E';
					break;
				case 15:
					character = 'F';
					break;
				default:
					character = hex + '0';
					break;
			}

			_k_screen_write_char_color( character, flags );
		}
	}

	if(is_cero)
	{
		_k_screen_write_char_color( '0', flags );
	}
}










__KFUNCTION__ void _k_screen_write_dword_hex( dword dw )
{
	_k_screen_write_dword_hex_color( dw, video_flags );
}