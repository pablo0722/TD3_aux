#ifndef _PDT_H_
#define _PDT_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/
#define CANT_PDES 4
#define CANT_PTES 1024










/************************************************
 * 					ENUM						*
 ************************************************/










/************************************************
 * 					ESTRUCTURAS					*
 ************************************************/
typedef struct
{
	dword none1: 	3;	// Ignorado
	dword pwt: 		1;	// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	dword pcd: 		1;	// Page Cache Disable (0: cache deshabilitado, 1: cache habilitado)
	dword none2:	7;	// Ignorado
	dword pde: 		20; // Comienzo de tabla de PDEs
} CR3; // CR3 register format

typedef struct
{
	dword p: 		1;	// Present
	dword rw: 		1;	// Read/Write (0: readable, 1: writable)
	dword us: 		1;	// User/Supervisor (0: kernel, 1: usuario)
	dword pwt: 		1;	// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	dword pcd: 		1;	// Page Cache Disable (0: cache deshabilitado, 1: cache habilitado)
	dword a: 		1;	// Accessed
	dword none1: 	1;	// Ignorado
	dword ps: 		1;	// Page Size (0: página de 4KB, 1: página de 4MB)

	dword none2:	4;	// Ignorado
	dword pte:		20;	// Comienzo de tabla de páginas
} PDE; // Page Directory Entry

typedef struct
{
	dword p: 	1;	// Present
	dword rw: 	1;	// Read/Write (0: readable, 1: writable)
	dword us: 	1;	// User/Supervisor (0: kernel, 1: usuario)
	dword pwt: 	1;	// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	dword pcd: 	1;	// Page Cache Disable (0: cache deshabilitado, 1: cache habilitado)
	dword a: 	1;	// Accessed
	dword d: 	1;	// Dirty
	dword pat: 	1;	// Page Attribute Table

	dword g: 	1;	// Global (1: NO se borra al cambiar CR3 (al cambiar de tarea), 0: Se borra al cambiar CR3)
	dword none:	3;	// Ignorado
	dword page: 20; // Comienzo de la página
} PTE; // Page Table Entry










/************************************************
 * 					SYS_TABLES 					*
 ************************************************/
extern PDE pde 						__SYS_TABLES__ ALIGN(32);
extern PTE pte[CANT_PTES] 			__SYS_TABLES__ ALIGN(32);










/************************************************
 * 					DATA 	 					*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _cr3_init();
__FUNCTION__ void _pde_init();
__FUNCTION__ void _pte_init();

#endif // _PDT_H_