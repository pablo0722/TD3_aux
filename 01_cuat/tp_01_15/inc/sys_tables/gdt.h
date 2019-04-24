#ifndef _GDT_H_
#define _GDT_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/
extern const dword CODE_SEL0;
extern const dword DATA_SEL0;
extern const dword CODE_SEL3;
extern const dword DATA_SEL3;
extern const dword TSS_SEL;










/************************************************
 * 					ENUM						*
 ************************************************/










/************************************************
 * 					ESTRUCTURAS					*
 ************************************************/
typedef struct
{
} GdtGate; // IDT (Interrupt / Trap / Task) Gate

typedef struct PACKED
{
	word length; 	// Tamaño de la tabla en reg 16 bits
	dword addr; 	// Dirección relativa en reg 32 bits
} Gdtr; // IDT register










/************************************************
 * 					SYS_TABLES 					*
 ************************************************/
extern Gdtr 	gdtr 				__SYS_TABLES__;










/************************************************
 * 					DATA 	 					*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
extern void _load_gdt();

#endif // _GDT_H_