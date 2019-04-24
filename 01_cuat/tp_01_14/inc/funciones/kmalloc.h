#ifndef _KMALLOC_H_
#define _KMALLOC_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "sys_tables/pdt.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KFUNCTION__ void kmalloc ( const void * page_dir_lin, word page_attr, PDE* pdt, PTE* pt );










#endif // _KMALLOC_H_