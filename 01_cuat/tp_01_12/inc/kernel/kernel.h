#ifndef _KERNEL_H_
#define _KERNEL_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/sys_types.h"
#include "kernel/basics.h"
#include "kernel/kernel_init.h"










/************************************************
 * 					VARIABLES 					*
 ************************************************/
extern dword key __BSS__;










/************************************************
 * 					DEFINES 					*
 ************************************************/
// RAM = 0x20000000 = 512MB
#define RAM 0x20000000










#endif // _KERNEL_H_