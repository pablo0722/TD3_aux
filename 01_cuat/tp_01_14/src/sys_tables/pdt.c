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
extern const dword __PDT_TAREA0_LIN;

extern const dword __PDT_TAREA0_PHY;
extern const dword __PDT_TAREA1_PHY;
extern const dword __PDT_TAREA2_PHY;










/************************************************
 * 					SYS_TABLES 					*
 ************************************************/
PDE * pdt_tarea0	__MY_DATA__ ALIGN(4) = (PDE *) &__PDT_TAREA0_PHY;
PTE * pt_tarea0 	__MY_DATA__ ALIGN(4) = (PTE *) &__PDT_TAREA0_PHY + CANT_PTE;

PDE * pdt_tarea1	__MY_DATA__ ALIGN(4) = (PDE *) &__PDT_TAREA1_PHY;
PTE * pt_tarea1 	__MY_DATA__ ALIGN(4) = (PTE *) &__PDT_TAREA1_PHY + CANT_PTE;

PDE * pdt_tarea2	__MY_DATA__ ALIGN(4) = (PDE *) &__PDT_TAREA2_PHY;
PTE * pt_tarea2 	__MY_DATA__ ALIGN(4) = (PTE *) &__PDT_TAREA2_PHY + CANT_PTE;










/************************************************
 * 					DATA 	 					*
 ************************************************/
// BSS

// DATA










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KFUNCTION__ void _pde_load(PDE *pdt, dword pde, const PTE *pt, word page_attr)
{
	pdt[pde].p 		= 1; 						// Present
	pdt[pde].rw 	= 1; 	// Read/Write (0: read only, 1: read/write)
	pdt[pde].us 	= 1; 	// User/Supervisor (0: kernel, 1: usuario)
	pdt[pde].pwt 	= 0; 						// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	pdt[pde].pcd 	= 1; 						// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
	pdt[pde].a 		= 0; 						// Accessed
	pdt[pde].none1	= 0; 						// Ignorado
	pdt[pde].ps 	= 0; 						// Page Size (0: página de 4KB, 1: página de 4MB)
	pdt[pde].none2	= 0; 						// Ignorado
	pdt[pde].pt 	= ((dword) pt) >> 12; 		// Comienzo de tabla de páginas
}










__KFUNCTION__ void _pte_load(PTE *pt, dword pte, const void *p, word page_attr)
{
	pt[pte].p 		= 1; 						// Present
	pt[pte].rw 		= (page_attr & 0x2) >> 1; 	// Read/Write (0: read only, 1: read/write)
	//pt[pte].rw 		= 1; 						// Read/Write (0: read only, 1: read/write)
	pt[pte].us 		= (page_attr & 0x4) >> 2; 	// User/Supervisor (0: kernel, 1: usuario)
	//pt[pte].us 		= 1; 						// User/Supervisor (0: kernel, 1: usuario)
	pt[pte].pwt 	= 0; 						// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	pt[pte].pcd 	= 1; 						// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
	pt[pte].a 		= 0; 						// Accessed
	pt[pte].d 		= 0; 						// Dirty
	pt[pte].pat 	= 0; 						// Page Attribute Table
	pt[pte].g 		= 0; 						// Global (1: NO se borra al cambiar CR3 (al cambiar de tarea), 0: Se borra al cambiar CR3)
	pt[pte].none 	= 0; 						// Ignorado
	pt[pte].page 	= ((dword) p) >> 12;		// Comienzo de la página
}










__KFUNCTION__ void _cr3_load(PDE *pdt)
{
	static CR3 cr3 __SYS_TABLES__;

	cr3.bits.pde 	= ((dword) pdt) >> 12;

	cr3.bits.none1 	= 0;
	cr3.bits.pwt 	= 0; 	// Page Write Through (0: PWT deshabilitado, 1: PWT habilitado)
	cr3.bits.pcd 	= 1; 	// Page Cache Disable (1: cache deshabilitado, 0: cache habilitado)
	cr3.bits.none2 	= 0;

	_set_cr3(cr3);
}










__KFUNCTION__ static void _pagine_error ( PDE * pdt, PTE * pt, const void * addr_lin )
{
	_k_screen_write_str_nl_color("_pagine_error\n", FG_RED | INTENSE);
	MBP();
}










__KFUNCTION__ void _pagine ( PDE * pdt, PTE * pt, const void * addr_phy, const void * addr_lin, word page_attr )
{
	dword pde = ( ( dword ) addr_lin >> 22 ) & 0x3FF;
	dword pte = ( ( dword ) addr_lin >> 12 ) & 0x3FF;
	pt = pt + pde * CANT_PTE;

	if ( pdt[pde].p == 1 )
	{
		// pde ya cargada. reviso los atributos

		if ( ( pdt[pde].rw == 0 ) && ( ( page_attr & 0x2 ) != 0 ) ) // Read/Write (0: read only, 1: read/write)
		{
			return _pagine_error(pdt, pt, addr_lin);
		}

		if ( ( pdt[pde].us == 0 ) && ( ( page_attr & 0x4 ) != 0 ) ) // User/Supervisor (0: kernel, 1: usuario)
		{
			return _pagine_error(pdt, pt, addr_lin);
		}

			// Si la direccion de pagina es distinta mando error, sino no hago nada
		if ( pdt[pde].pt != ( ( dword ) pt >> 12 ) )
		{
			return _pagine_error(pdt, pt, addr_lin);
		}


		if ( pt[pte].p == 1 )
		{
			// pde ya cargada. reviso los atributos

			if ( ( pt[pte].rw == 0 ) && ( ( page_attr & 0x2 ) != 0 ) ) // Read/Write (0: read only, 1: read/write)
			{
				return _pagine_error(pdt, pt, addr_lin);
			}

			if ( ( pt[pte].us == 0 ) && ( ( page_attr & 0x4 ) != 0 ) ) // User/Supervisor (0: kernel, 1: usuario)
			{
				return _pagine_error(pdt, pt, addr_lin);
			}

			// Si la direccion de pagina es distinta mando error, sino no hago nada
			if ( pt[pte].page != ( ( dword ) addr_phy >> 12 ) )
			{
				return _pagine_error(pdt, pt, addr_lin);
			}

			return;
		}
	}
	else
	{
		// pde no cargada.
		
		_pde_load ( pdt, pde, pt, page_attr );
	}

	_pte_load ( pt, pte, addr_phy, page_attr );
}










__KFUNCTION__ word _pagine_get_attr ( const void * addr_lin )
{
	word 	page_attr;
	PTE * 	pt 			= ( PTE * ) ( &__PDT_TAREA0_LIN + CANT_PTE );
	dword 	pde 		= ( ( dword ) addr_lin >> 22 ) & 0x3FF;
	dword 	pte 		= ( ( dword ) addr_lin >> 12 ) & 0x3FF;
	pt 					= pt + pde * CANT_PTE;

	page_attr = pt[pte].rw << 0x01; 	// Read/Write (0: read only, 1: read/write)
	page_attr = pt[pte].us << 0x02; 	// User/Supervisor (0: kernel, 1: usuario)
	
	//return page_attr;	
	return 0xF;
}