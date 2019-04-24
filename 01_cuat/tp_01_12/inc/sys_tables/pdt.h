#ifndef _PDT_H_
#define _PDT_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/
#define CANT_PDE 128
#define CANT_PTE 1024

#define PAGE_ATTR_R 		0
#define PAGE_ATTR_RW 		2
#define PAGE_ATTR_KERNEL 	0
#define PAGE_ATTR_USER 		4










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
	dword pcd: 		1;	// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
	dword none2:	7;	// Ignorado
	dword pde: 		20; // Comienzo de tabla de PDEs
} CR3; // CR3 register format

typedef struct
{
	dword p: 		1;	// Present
	dword rw: 		1;	// Read/Write (0: read only, 1: read/write)
	dword us: 		1;	// User/Supervisor (0: kernel, 1: usuario)
	dword pwt: 		1;	// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	dword pcd: 		1;	// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
	dword a: 		1;	// Accessed
	dword none1: 	1;	// Ignorado
	dword ps: 		1;	// Page Size (0: página de 4KB, 1: página de 4MB)

	dword none2:	4;	// Ignorado
	dword pt:		20;	// Comienzo de tabla de páginas
} PDE; // Page Directory Entry

typedef struct
{
	dword p: 	1;	// Present
	dword rw: 	1;	// Read/Write (0: read only, 1: read/write)
	dword us: 	1;	// User/Supervisor (0: kernel, 1: usuario)
	dword pwt: 	1;	// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	dword pcd: 	1;	// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
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
extern PDE * pdt	__MY_DATA__ ALIGN(4);
extern PTE * pt 	__MY_DATA__ ALIGN(4);










/************************************************
 * 					DATA 	 					*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _pdt_ptr_load();
__FUNCTION__ void _pde_load(PDE *pdt, dword pde, const PTE *pt, word page_attr);
__FUNCTION__ void _pte_load(PTE *pt, dword pte, const void *p, word page_attr);
__FUNCTION__ void _cr3_load(PDE *pdt);
__FUNCTION__ void _pagine(PDE *pdt, PTE *pt, const void *addr_phy, const void *addr_lin, word page_attr);

#endif // _PDT_H_