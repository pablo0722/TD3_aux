#ifndef _BASICS_H_
#define _BASICS_H_










/************************************************
 * 					DEFINES	 					*
 ************************************************/
#define MBP() 				asm("xchg %%bx, %%bx"::)
#define ISR_DEFAULT(value) 	asm("mov %0, %%dx" : : "r" ((word)value)); \
							asm("hlt"::)











#endif  /*_BASICS_H_*/
