#ifndef _BASICS_H_
#define _BASICS_H_

/************************************************
 * 					DEFINES	 					*
 ************************************************/
// Secciones
#define __ISR__ 			__attribute__(( section(".isr")))
#define __INIT16__ 			__attribute__(( section(".init16")))
#define __KERNEL__ 			__attribute__(( section(".kernel32")))
#define __FUNCTION__ 		__attribute__(( section(".funciones")))
#define __MY_BSS__ 			__attribute__(( section(".my_bss")))
#define __MY_DATA__ 		__attribute__(( section(".my_data")))
#define __SYS_TABLES__ 		__attribute__(( section(".sys_tables")))
#define __PAG_TABLES__ 		__attribute__(( section(".pag_tables")))
#define __KEYBOARD__ 		__attribute__(( section(".keyboard_buffer")))
#define __TABLA_DIGITOS__ 	__attribute__(( section(".tabla_digitos")))
#define __TSS_GLOBAL__ 		__attribute__(( section(".bss.tss_global")))
#define __TSS_TAREA0__ 		__attribute__(( section(".bss.tss_tarea0")))
#define __TSS_TAREA1__ 		__attribute__(( section(".bss.tss_tarea1")))
#define __TSS_TAREA2__ 		__attribute__(( section(".bss.tss_tarea2")))

#define __RODATA__ 			__attribute__(( section(".rodata")))
#define __BSS__ 			__attribute__(( section(".bss")))
#define __DATA__ 			__attribute__(( section(".data")))
#define __TEXT__ 			__attribute__(( section(".text")))

// Atributos
#define PACKED				__attribute__((packed))
#define ALIGN(alignment) 	__attribute__((aligned (alignment)))

// Assembler
#define MBP() 				asm("xchg %%bx, %%bx"::)
#define ISR_DEFAULT(value) 	asm("mov %0, %%dx" : : "r" ((word)value)); \
							asm("hlt"::)

// Otros
#define NULL 				((void*) 0)











#endif  // _BASICS_H
