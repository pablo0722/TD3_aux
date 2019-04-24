#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_










/************************************************
 * 					DEFINES	 					*
 ************************************************/
// Secciones
	#define __ISR__ 			__attribute__(( section(".isr")))
	#define __INIT16__ 			__attribute__(( section(".init16")))
	#define __KERNEL__ 			__attribute__(( section(".kernel32")))
	#define __FUNCTION__ 		__attribute__(( section(".funciones")))
	#define __BSS__ 			__attribute__(( section(".my_bss")))
	#define __DATA__ 			__attribute__(( section(".my_data")))
	#define __SYS_TABLES__ 		__attribute__(( section(".sys_tables")))

#define PACKED				__attribute__((packed))
#define ALIGN(alignment) 	__attribute__((aligned (alignment)))
#define NULL 				((void*) 0)










/************************************************
 * 					TYPEDEFS 					*
 ************************************************/
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long long qword;
typedef qword ticks_t;











#endif  /*_SYS_TYPES_H_*/
