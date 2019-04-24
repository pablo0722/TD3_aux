#ifndef _INTERRUPCIONES_H_
#define _INTERRUPCIONES_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
// Funciones en assembler
extern void _IrqTM0();
extern void _IrqKB();

// Funciones en C
__ISR__ void _isr_tm0(); 	// KeyBoard IRQ 1 del PIC 1
__ISR__ void _isr_kb(dword key); 	// KeyBoard IRQ 1 del PIC 1

#endif // _INTERRUPCIONES_H_