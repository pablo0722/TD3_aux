#ifndef _SCREEN_H_
#define _SCREEN_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
extern __FUNCTION__ void _screen_clean(dword row_size);
__FUNCTION__ void write_string( int colour, const char *string );










#endif // _SCREEN_H_