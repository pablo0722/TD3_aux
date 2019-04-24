#ifndef _EXCEPCIONES_H_
#define _EXCEPCIONES_H_

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
extern void _IrqDE();
extern void _IrqNMI();
extern void _IrqBP();
extern void _IrqOF();
extern void _IrqBR();
extern void _IrqUD();
extern void _IrqNM();
extern void _IrqDF();
extern void _IrqTS();
extern void _IrqNP();
extern void _IrqSS();
extern void _IrqGP();
extern void _IrqPF();
extern void _IrqMF();
extern void _IrqAC();
extern void _IrqMC();
extern void _IrqXF();

// Funciones en C
__ISR__ void _isr_de(); 	// Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
__ISR__ void _isr_nmi();
__ISR__ void _isr_bp();
__ISR__ void _isr_of();
__ISR__ void _isr_br();
__ISR__ void _isr_ud(); 	// Invalid Opcode Exception
__ISR__ void _isr_nm();
__ISR__ void _isr_df(); 	// Doble Fault
__ISR__ void _isr_ts();
__ISR__ void _isr_np();
__ISR__ void _isr_ss();
__ISR__ void _isr_gp(); 	// General Protection
__ISR__ void _isr_pf(); 	// Page Fault
__ISR__ void _isr_mf();
__ISR__ void _isr_ac();
__ISR__ void _isr_mc();
__ISR__ void _isr_xf();

#endif // _EXCEPCIONES_H_