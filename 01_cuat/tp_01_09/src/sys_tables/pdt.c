/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "sys_tables/pdt.h"

#include "kernel/kernel.h"
#include "funciones/funciones.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					SYS_TABLES 					*
 ************************************************/
PDE pde 					__SYS_TABLES__ ALIGN(4);
PTE pte[CANT_PTES] 			__SYS_TABLES__ ALIGN(4);










/************************************************
 * 					DATA 	 					*
 ************************************************/
// BSS

// DATA










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _cr3_init()
{
	MBP();
	CR3 cr3;

	cr3.pde 	= (dword) &pde;

	cr3.none1 	= 0;
	cr3.pwt 	= 0;
	cr3.pcd 	= 0;
	cr3.none2 	= 0;

	_load_cr3(cr3);
}

__FUNCTION__ void _pde_init()
{
	pde.p 		= 1; 					// Present
	pde.rw 		= 1; 					// Read/Write (0: readable, 1: writable)
	pde.us 		= 0; 					// User/Supervisor (0: kernel, 1: usuario)
	pde.pwt 	= 0; 					// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	pde.pcd 	= 0; 					// Page Cache Disable (0: cache deshabilitado, 1: cache habilitado)
	pde.a 		= 1; 					// Accessed
	pde.none1	= 0; 					// Ignorado
	pde.ps 		= 0; 					// Page Size (0: página de 4KB, 1: página de 4MB)
	pde.none2	= 0; 					// Ignorado
	pde.pte 	= ((dword) pte) >> 12; 	// Comienzo de tabla de páginas
}

__FUNCTION__ void _pte_init()
{
	int i;

	for(i=0; i<CANT_PTES; i++)
	{
		pte[i].p 	= 1; 	// Present
		pte[i].rw 	= 1; 	// Read/Write (0: readable, 1: writable)
		pte[i].us 	= 0; 	// User/Supervisor (0: kernel, 1: usuario)
		pte[i].pwt 	= 0; 	// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
		pte[i].pcd 	= 0; 	// Page Cache Disable (0: cache deshabilitado, 1: cache habilitado)
		pte[i].a 	= 1; 	// Accessed
		pte[i].d 	= 0; 	// Dirty
		pte[i].pat 	= 0; 	// Page Attribute Table
		pte[i].g 	= 0; 	// Global (1: NO se borra al cambiar CR3 (al cambiar de tarea), 0: Se borra al cambiar CR3)
		pte[i].none	= 0; 	// Ignorado
		pte[i].page = i;	// Comienzo de la página
	}
}