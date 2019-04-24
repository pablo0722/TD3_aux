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
extern void _IrsSC();

// Funciones en C
__ISR__ void _isr_tm0(); 								// Timer 	IRQ 0 del PIC 1
__ISR__ void _isr_kb(dword key); 						// KeyBoard IRQ 1 del PIC 1
__ISR__ dword _isr_system_call(dword func, dword arg1, dword arg2); 	// System Call

#endif // _INTERRUPCIONES_H_