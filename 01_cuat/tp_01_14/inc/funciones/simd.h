#ifndef _SCREEN_H_
#define _SCREEN_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _simd_suma_desborde_byte( const dword * buffer, dword *accum, dword cant );
__FUNCTION__ void _simd_suma_desborde_dword( const dword * buffer, dword *accum, dword cant );










#endif // _SCREEN_H_