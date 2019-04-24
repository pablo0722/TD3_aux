/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel_init.h"

#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"
#include "isr/isr.h"
#include "funciones/funciones.h"
#include "tareas/tareas.h"










/************************************************
 * 					DEFINES LINKER				*
 ************************************************/
extern byte __SHDW_ISR_LIN;
extern byte __SHDW_ISR_LMA;
extern byte __SHDW_ISR_LEN;
extern byte __SHDW_ISR_PHY;

extern byte __SHDW_GDT_LIN;
extern byte __SHDW_GDT_LMA;
extern byte __SHDW_GDT_LEN;
extern byte __SHDW_GDT_PHY;

extern byte __SHDW_SYS_TABLES_LIN;
extern byte __SHDW_SYS_TABLES_LMA;
extern byte __SHDW_SYS_TABLES_LEN;
extern byte __SHDW_SYS_TABLES_PHY;

extern byte __SHDW_KERNEL32_LEN;
extern byte __SHDW_KERNEL32_LIN;
extern byte __SHDW_KERNEL32_PHY;

extern byte __SHDW_DATA_PHY;
extern byte __SHDW_DATA_LIN;
extern byte __SHDW_DATA_LMA;
extern byte __SHDW_DATA_LEN;

extern byte __SHDW_FUNC32_PHY;
extern byte __SHDW_FUNC32_LIN;
extern byte __SHDW_FUNC32_LMA;
extern byte __SHDW_FUNC32_LEN;

extern byte __TABLA_DIGITOS_PHY;
extern byte __TABLA_DIGITOS_LIN;
extern byte __TABLA_DIGITOS_LEN;

extern byte __KEYBOARD_BUFFER_PHY;
extern byte __KEYBOARD_BUFFER_LIN;
extern byte __KEYBOARD_BUFFER_LEN;

extern byte __PDT_START;
extern byte __PDT_LEN;
extern byte __PDT_END;

extern byte __STACK_START_32;
extern byte __STACK_SIZE_32;

extern byte __STACK_TAREA_NIVEL3_LIN;
extern byte __STACK_TAREA0_NIVEL3_PHY;
extern byte __STACK_TAREA1_NIVEL3_PHY;
extern byte __STACK_TAREA2_NIVEL3_PHY;

extern byte __STACK_TAREA_NIVEL0_LIN;
extern byte __STACK_TAREA_NIVEL0_LEN;
extern byte __STACK_TAREA0_NIVEL0_PHY;
extern byte __STACK_TAREA1_NIVEL0_PHY;
extern byte __STACK_TAREA2_NIVEL0_PHY;
extern byte __STACK_TAREA0_NIVEL0_LMA;
extern byte __STACK_TAREA1_NIVEL0_LMA;
extern byte __STACK_TAREA2_NIVEL0_LMA;

extern byte __TSS_GLOBAL_LIN;
extern byte __TSS_GLOBAL_LEN;
extern byte __TSS_GLOBAL_LMA;
extern byte __TSS_GLOBAL_PHY;

extern byte __TAREA0_TEXT_LMA;
extern byte __TAREA1_TEXT_LMA;
extern byte __TAREA2_TEXT_LMA;
extern byte __TAREA0_TEXT_PHY;
extern byte __TAREA1_TEXT_PHY;
extern byte __TAREA2_TEXT_PHY;
extern byte __TAREA0_TEXT_LEN;
extern byte __TAREA1_TEXT_LEN;
extern byte __TAREA2_TEXT_LEN;
extern byte __TAREA_TEXT_LIN;

extern byte __TAREA0_BSS_PHY;
extern byte __TAREA1_BSS_PHY;
extern byte __TAREA2_BSS_PHY;
extern byte __TAREA0_BSS_LEN;
extern byte __TAREA1_BSS_LEN;
extern byte __TAREA2_BSS_LEN;
extern byte __TAREA_BSS_LIN;

extern byte __TAREA0_DATA_LMA;
extern byte __TAREA1_DATA_LMA;
extern byte __TAREA2_DATA_LMA;
extern byte __TAREA0_DATA_PHY;
extern byte __TAREA1_DATA_PHY;
extern byte __TAREA2_DATA_PHY;
extern byte __TAREA0_DATA_LEN;
extern byte __TAREA1_DATA_LEN;
extern byte __TAREA2_DATA_LEN;
extern byte __TAREA_DATA_LIN;

extern byte __PDT_TAREA0_LIN;
extern byte __PDT_TAREA1_LIN;
extern byte __PDT_TAREA2_LIN;
extern byte __PDT_TAREA0_PHY;
extern byte __PDT_TAREA1_PHY;
extern byte __PDT_TAREA2_PHY;
extern byte __PDT_TAREA_LEN;










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					LOCAL FORWARDS				*
 ************************************************/
__KERNEL__ static void _shadow_copy();
__KERNEL__ static void _gdt_init();
__KERNEL__ static void _idt_init();
__KERNEL__ static void _pdt_init();
__KERNEL__ static void _pdt_tarea0_init();
__KERNEL__ static void _pdt_tarea1_init();
__KERNEL__ static void _pdt_tarea2_init();










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KERNEL__ void _kernel_init()
{
	_screen_write_str("_kernel_init\n");

	_shadow_copy();

	_ring_buffer_init(&ring_buffer_tabla_digitos);
	
	_ring_buffer_init(&ring_buffer_keyboard);

	_gdt_init();

	_idt_init();

	_pdt_init();
}











__KERNEL__ static void _shadow_copy()
{
	dword offset;

	_memcpy(&__SHDW_ISR_PHY, 			&__SHDW_ISR_LMA, 			(dword) &__SHDW_ISR_LEN);
	_memcpy(&__SHDW_GDT_PHY, 			&__SHDW_GDT_LMA, 			(dword) &__SHDW_GDT_LEN);
	_memcpy(&__SHDW_SYS_TABLES_PHY, 	&__SHDW_SYS_TABLES_LMA, 	(dword) &__SHDW_SYS_TABLES_LEN);
	_memcpy(&__TSS_GLOBAL_PHY, 			&__TSS_GLOBAL_LMA, 			(dword) &__TSS_GLOBAL_LEN);
	_memcpy(&__SHDW_DATA_PHY,  			&__SHDW_DATA_LMA, 			(dword) &__SHDW_DATA_LEN);
	_memcpy(&__STACK_TAREA0_NIVEL0_PHY, &__STACK_TAREA0_NIVEL0_LMA, (dword) &__STACK_TAREA_NIVEL0_LEN);
	_memcpy(&__STACK_TAREA1_NIVEL0_PHY, &__STACK_TAREA1_NIVEL0_LMA, (dword) &__STACK_TAREA_NIVEL0_LEN);
	_memcpy(&__STACK_TAREA2_NIVEL0_PHY, &__STACK_TAREA2_NIVEL0_LMA, (dword) &__STACK_TAREA_NIVEL0_LEN);

	// TAREA 0
	_memcpy(&__TAREA0_TEXT_PHY, &__TAREA0_TEXT_LMA, (dword) &__TAREA0_TEXT_LEN);
	_memcpy(&__TAREA0_DATA_PHY, &__TAREA0_DATA_LMA, (dword) &__TAREA0_DATA_LEN);

	for(offset = 0; offset < (dword) &__TAREA0_BSS_LEN; offset++)
	{
		*((&__TAREA0_BSS_PHY) + offset) = 0;
	}
	
	// TAREA 1
	_memcpy(&__TAREA1_TEXT_PHY, 	&__TAREA1_TEXT_LMA, 	(dword) &__TAREA1_TEXT_LEN);
	_memcpy(&__TAREA1_DATA_PHY, 	&__TAREA1_DATA_LMA, 	(dword) &__TAREA1_DATA_LEN);

	for(offset = 0; offset <= (dword) &__TAREA1_BSS_LEN; offset++)
	{
		*((&__TAREA1_BSS_PHY) + offset) = 0;
	}
	
	// TAREA 2
	_memcpy(&__TAREA2_TEXT_PHY, 	&__TAREA2_TEXT_LMA, 	(dword) &__TAREA2_TEXT_LEN);
	_memcpy(&__TAREA2_DATA_PHY, 	&__TAREA2_DATA_LMA, 	(dword) &__TAREA2_DATA_LEN);

	for(offset = 0; offset <= (dword) &__TAREA2_BSS_LEN; offset++)
	{
		*((&__TAREA2_BSS_PHY) + offset) = 0;
	}
}











__KERNEL__ static void _gdt_init()
{
	_load_gdt(gdtr);
}











__KERNEL__ static void _idt_init()
{
	_isr_load(&(idt_gates[IRQ_DE]), 	&_IrqDE, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	// _isr_load(&(idt_gates[IRQ1]), 		NULL, 0, 0);	
	_isr_load(&(idt_gates[IRQ_NMI]), 	&_IrqNMI, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_BP]), 	&_IrqBP, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_OF]), 	&_IrqOF, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_BR]), 	&_IrqBR, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_UD]), 	&_IrqUD, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_NM]), 	&_IrqNM, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_DF]), 	&_IrqDF, 	(dword) &CODE_SEL0, IDT_TYPE_TASK);
	// _isr_load(&(idt_gates[IRQ9]), 		NULL, 0, 0);
	_isr_load(&(idt_gates[IRQ_TS]), 	&_IrqTS, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_NP]), 	&_IrqNP, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_SS]), 	&_IrqSS, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_GP]), 	&_IrqGP, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_PF]), 	&_IrqPF, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	// _isr_load(&(idt_gates[IRQ15]), 		NULL, 0, 0);
	_isr_load(&(idt_gates[IRQ_MF]), 	&_IrqMF, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_AC]), 	&_IrqAC, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_MC]), 	&_IrqMC, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	_isr_load(&(idt_gates[IRQ_XF]), 	&_IrqXF, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
	// _isr_load(&(idt_gates[IRQ20]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ21]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ22]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ23]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ24]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ25]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ26]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ27]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ28]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ29]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ30]), 		NULL, 0, 0);
	// _isr_load(&(idt_gates[IRQ31]), 		NULL, 0, 0);
	_isr_load(&(idt_gates[IRQ_TM0]), 	&_IrqTM0, 	(dword) &CODE_SEL0, IDT_TYPE_INT);
	_isr_load(&(idt_gates[IRQ_KB]), 	&_IrqKB, 	(dword) &CODE_SEL0, IDT_TYPE_INT);

	idtr.length = IRQ_CANT * sizeof(IdtGate) - 1;
	idtr.addr = (dword) idt_gates;
}











__KERNEL__ static void _pdt_init()
{
	dword offset;

	_screen_write_str("_pdt_init\n");

	_screen_write_str("stack\n");
	for(offset = 0x00000000; offset <= ((dword) &__STACK_SIZE_32)*4; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__STACK_START_32 + offset), (&__STACK_START_32 + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__STACK_START_32 + offset), (&__STACK_START_32 + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__STACK_START_32 + offset), (&__STACK_START_32 + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("isr\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_ISR_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__SHDW_ISR_PHY + offset), (&__SHDW_ISR_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__SHDW_ISR_PHY + offset), (&__SHDW_ISR_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__SHDW_ISR_PHY + offset), (&__SHDW_ISR_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("gdt\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_GDT_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__SHDW_GDT_PHY + offset), (&__SHDW_GDT_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__SHDW_GDT_PHY + offset), (&__SHDW_GDT_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__SHDW_GDT_PHY + offset), (&__SHDW_GDT_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("sys_tab\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_SYS_TABLES_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__SHDW_SYS_TABLES_PHY + offset), (&__SHDW_SYS_TABLES_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__SHDW_SYS_TABLES_PHY + offset), (&__SHDW_SYS_TABLES_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__SHDW_SYS_TABLES_PHY + offset), (&__SHDW_SYS_TABLES_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("K32\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_KERNEL32_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__SHDW_KERNEL32_PHY + offset), (&__SHDW_KERNEL32_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__SHDW_KERNEL32_PHY + offset), (&__SHDW_KERNEL32_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__SHDW_KERNEL32_PHY + offset), (&__SHDW_KERNEL32_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("data\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_DATA_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__SHDW_DATA_PHY + offset), (&__SHDW_DATA_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__SHDW_DATA_PHY + offset), (&__SHDW_DATA_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__SHDW_DATA_PHY + offset), (&__SHDW_DATA_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("F32\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_FUNC32_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__SHDW_FUNC32_PHY + offset), (&__SHDW_FUNC32_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__SHDW_FUNC32_PHY + offset), (&__SHDW_FUNC32_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__SHDW_FUNC32_PHY + offset), (&__SHDW_FUNC32_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("dig_tab\n");
	for(offset = 0x00000000; offset < (dword) &__TABLA_DIGITOS_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__TABLA_DIGITOS_PHY + offset), (&__TABLA_DIGITOS_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__TABLA_DIGITOS_PHY + offset), (&__TABLA_DIGITOS_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__TABLA_DIGITOS_PHY + offset), (&__TABLA_DIGITOS_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("KB\n");
	for(offset = 0x00000000; offset < (dword) &__KEYBOARD_BUFFER_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__KEYBOARD_BUFFER_PHY + offset), (&__KEYBOARD_BUFFER_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea1, pt_tarea1, (&__KEYBOARD_BUFFER_PHY + offset), (&__KEYBOARD_BUFFER_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
		_pagine(pdt_tarea2, pt_tarea2, (&__KEYBOARD_BUFFER_PHY + offset), (&__KEYBOARD_BUFFER_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_screen_write_str("vga\n");
	_pagine(pdt_tarea0, pt_tarea0, (const void *) FRAME_BUFFER, (const void *) FRAME_BUFFER, PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	_pagine(pdt_tarea1, pt_tarea1, (const void *) FRAME_BUFFER, (const void *) FRAME_BUFFER, PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	_pagine(pdt_tarea2, pt_tarea2, (const void *) FRAME_BUFFER, (const void *) FRAME_BUFFER, PAGE_ATTR_KERNEL | PAGE_ATTR_RW);



	// TAREAS
	_screen_write_str("t0\n");
 	_pdt_tarea0_init();

	_screen_write_str("t1\n");
 	_pdt_tarea1_init();

	_screen_write_str("t2\n");
 	_pdt_tarea2_init();



	// HABILITAR PAGINACION
	_screen_write_str("Habilit pag\n");
	_cr3_load(pdt_tarea0);
	_paging_enable();
	_screen_reset();
}










__KERNEL__ static void _pdt_tarea0_init()
{
	dword offset;

	for(offset = 0x00000000; offset < (dword) &__PDT_TAREA_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea0, pt_tarea0, (&__PDT_TAREA0_PHY + offset), (&__PDT_TAREA0_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_pagine(pdt_tarea0, pt_tarea0, &__TAREA0_TEXT_PHY, 			&__TAREA_TEXT_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea0, pt_tarea0, &__TAREA0_BSS_PHY, 			&__TAREA_BSS_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea0, pt_tarea0, &__TAREA0_DATA_PHY, 			&__TAREA_DATA_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea0, pt_tarea0, &__STACK_TAREA0_NIVEL0_PHY,  &__STACK_TAREA_NIVEL0_LIN, 	PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea0, pt_tarea0, &__STACK_TAREA0_NIVEL3_PHY,  &__STACK_TAREA_NIVEL3_LIN, 	PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
}










__KERNEL__ static void _pdt_tarea1_init()
{
	dword offset;

	for(offset = 0x00000000; offset < (dword) &__PDT_TAREA_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea1, pt_tarea1, (&__PDT_TAREA1_PHY + offset), (&__PDT_TAREA1_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_pagine(pdt_tarea1, pt_tarea1, &__TAREA1_TEXT_PHY, 			&__TAREA_TEXT_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea1, pt_tarea1, &__TAREA1_BSS_PHY, 			&__TAREA_BSS_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea1, pt_tarea1, &__TAREA1_DATA_PHY, 			&__TAREA_DATA_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea1, pt_tarea1, &__STACK_TAREA1_NIVEL0_PHY,  &__STACK_TAREA_NIVEL0_LIN, 	PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea1, pt_tarea1, &__STACK_TAREA1_NIVEL3_PHY,  &__STACK_TAREA_NIVEL3_LIN, 	PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
}










__KERNEL__ static void _pdt_tarea2_init()
{
	dword offset;

	for(offset = 0x00000000; offset < (dword) &__PDT_TAREA_LEN; offset += 0x1000)
	{
		_pagine(pdt_tarea2, pt_tarea2, (&__PDT_TAREA2_PHY + offset), (&__PDT_TAREA2_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	_pagine(pdt_tarea2, pt_tarea2, &__TAREA2_TEXT_PHY, 			&__TAREA_TEXT_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea2, pt_tarea2, &__TAREA2_BSS_PHY, 			&__TAREA_BSS_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea2, pt_tarea2, &__TAREA2_DATA_PHY, 			&__TAREA_DATA_LIN, 			PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea2, pt_tarea2, &__STACK_TAREA2_NIVEL0_PHY,  &__STACK_TAREA_NIVEL0_LIN, 	PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
	_pagine(pdt_tarea2, pt_tarea2, &__STACK_TAREA2_NIVEL3_PHY,  &__STACK_TAREA_NIVEL3_LIN, 	PAGE_ATTR_KERNEL 	| PAGE_ATTR_RW);
}