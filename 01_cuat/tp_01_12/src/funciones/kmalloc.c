/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "funciones/kmalloc.h"

#include "kernel/kernel.h"
#include "sys_tables/pdt.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/









/************************************************
 * 					DATA						*
 ************************************************/
byte * new_page_phy __DATA__ = ( byte * ) 0x10000000;










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void kmalloc ( const void * page_dir_lin, word page_attr )
{
	_pagine( pdt, pt, ( const void * ) new_page_phy, page_dir_lin, page_attr );
	new_page_phy += 0x1000;
}