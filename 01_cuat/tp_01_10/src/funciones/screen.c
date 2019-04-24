/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "funciones/screen.h"

#include "kernel/kernel.h"
#include "funciones/funciones.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void write_string( int colour, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = colour;
    }
}