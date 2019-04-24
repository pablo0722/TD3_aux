#ifndef _LOW_LEVEL_H_
#define _LOW_LEVEL_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA 	 					*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _load_idt(Idtr idtr);
__FUNCTION__ void _load_cr3(CR3 cr3);
__FUNCTION__ void _hlt();


__FUNCTION__ void _IntGenDE();
__FUNCTION__ void _IntGenNMI();
__FUNCTION__ void _IntGenBP();
__FUNCTION__ void _IntGenOF();
__FUNCTION__ void _IntGenBR();
__FUNCTION__ void _IntGenUD();
__FUNCTION__ void _IntGenNM();
__FUNCTION__ void _IntGenDF();
__FUNCTION__ void _IntGenTS();
__FUNCTION__ void _IntGenNP();
__FUNCTION__ void _IntGenSS();
__FUNCTION__ void _IntGenGP();
__FUNCTION__ void _IntGenPF();
__FUNCTION__ void _IntGenMF();
__FUNCTION__ void _IntGenAC();
__FUNCTION__ void _IntGenMC();
__FUNCTION__ void _IntGenXF();

#endif // _LOW_LEVEL_H_