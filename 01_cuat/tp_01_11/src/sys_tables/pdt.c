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
 * 					DEFINES LINKER				*
 ************************************************/
extern const dword __PDT_START;










/************************************************
 * 					SYS_TABLES 					*
 ************************************************/
PDE * pdt	__MY_DATA__ ALIGN(4);
PTE * pt 	__MY_DATA__ ALIGN(4);










/************************************************
 * 					DATA 	 					*
 ************************************************/
// BSS

// DATA










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _pdt_ptr_load()
{
	pdt = (PDE *) &__PDT_START;
	pt  = (PTE *) &__PDT_START + CANT_PTE;
}










__FUNCTION__ void _pde_load(PDE *pdt, dword pde, const PTE *pt, word page_attr)
{
	pdt[pde].p 		= 1; 					// Present
	pdt[pde].rw 	= page_attr & 0x2; 		// Read/Write (0: read only, 1: read/write)
	pdt[pde].us 	= page_attr & 0x4; 		// User/Supervisor (0: kernel, 1: usuario)
	pdt[pde].pwt 	= 0; 					// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	pdt[pde].pcd 	= 1; 					// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
	pdt[pde].a 		= 0; 					// Accessed
	pdt[pde].none1	= 0; 					// Ignorado
	pdt[pde].ps 	= 0; 					// Page Size (0: página de 4KB, 1: página de 4MB)
	pdt[pde].none2	= 0; 					// Ignorado
	pdt[pde].pt 	= ((dword) pt) >> 12; 	// Comienzo de tabla de páginas
}










__FUNCTION__ void _pte_load(PTE *pt, dword pte, const void *p, word page_attr)
{
	pt[pte].p 		= 1; 					// Present
	pt[pte].rw 		= page_attr & 0x2; 		// Read/Write (0: read only, 1: read/write)
	pt[pte].us 		= page_attr & 0x4; 		// User/Supervisor (0: kernel, 1: usuario)
	pt[pte].pwt 	= 0; 					// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	pt[pte].pcd 	= 1; 					// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
	pt[pte].a 		= 0; 					// Accessed
	pt[pte].d 		= 0; 					// Dirty
	pt[pte].pat 	= 0; 					// Page Attribute Table
	pt[pte].g 		= 0; 					// Global (1: NO se borra al cambiar CR3 (al cambiar de tarea), 0: Se borra al cambiar CR3)
	pt[pte].none 	= 0; 					// Ignorado
	pt[pte].page 	= ((dword) p) >> 12;	// Comienzo de la página
}










__FUNCTION__ void _cr3_load(PDE *pdt)
{
	static CR3 cr3 __SYS_TABLES__;

	cr3.pde 	= ((dword) pdt) >> 12;

	cr3.none1 	= 0;
	cr3.pwt 	= 0; 	// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	cr3.pcd 	= 1; 	// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
	cr3.none2 	= 0;

	_load_cr3(cr3);
}










__FUNCTION__ void _pagine(PDE *pdt, PTE *pt, const void *addr_phy, const void *addr_lin, word page_attr)
{
	dword pde = ((dword) addr_lin >> 22) & 0x3FF;
	dword pte = ((dword) addr_lin >> 12) & 0x3FF;

	_pde_load(pdt, pde, pt+pde*CANT_PTE, page_attr);
	_pte_load(pt+pde*CANT_PTE, pte, addr_phy, page_attr);
}
