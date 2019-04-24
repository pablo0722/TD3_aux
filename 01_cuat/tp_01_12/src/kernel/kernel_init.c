/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel_init.h"

#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"
#include "isr/isr.h"
#include "funciones/funciones.h"










/************************************************
 * 					DEFINES LINKER				*
 ************************************************/
extern byte __SHDW_ISR_DST;
extern byte __SHDW_ISR_ORG;
extern byte __SHDW_ISR_LEN;

extern byte __SHDW_GDT_DST;
extern byte __SHDW_GDT_ORG;
extern byte __SHDW_GDT_LEN;

extern byte __SHDW_SYS_TABLES_DST;
extern byte __SHDW_SYS_TABLES_ORG;
extern byte __SHDW_SYS_TABLES_LEN;

extern byte __SHDW_KERNEL32_LEN;
extern byte __SHDW_KERNEL32_DST;

extern byte __SHDW_DATA_DST;
extern byte __SHDW_DATA_ORG;
extern byte __SHDW_DATA_LEN;

extern byte __TABLA_DIGITOS_LIN;
extern byte __TABLA_DIGITOS_LEN;

extern byte __KEYBOARD_BUFFER_LIN;
extern byte __KEYBOARD_BUFFER_LEN;

extern byte __PDT_START;
extern byte __PDT_LEN;
extern byte __PDT_END;

extern byte __STACK_START_32;
extern byte __STACK_SIZE_32;
extern byte __STACK_END_32;

extern byte __STACK_TAREA1_PHY;
extern byte __STACK_TAREA1_LIN;
extern byte __STACK_TAREA1_SIZE;

extern byte __SHDW_TAREA1_TEXT_DST;
extern byte __SHDW_TAREA1_TEXT_PHY;
extern byte __SHDW_TAREA1_BSS_DST;
extern byte __SHDW_TAREA1_BSS_PHY;
extern byte __SHDW_TAREA1_BSS_LEN;
extern byte __SHDW_TAREA1_DATA_DST;
extern byte __SHDW_TAREA1_DATA_PHY;










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					LOCAL FORWARDS				*
 ************************************************/
__KERNEL__ static void _shadow_copy();
__KERNEL__ static void _bss_init();
__KERNEL__ static void _gdt_init();
__KERNEL__ static void _idt_init();
__KERNEL__ static void _pdt_init();










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KERNEL__ void _kernel_init()
{
	screen_write_str("_kernel_init\n");

	_shadow_copy();

	_bss_init();
	
	_ring_buffer_init(&ring_buffer_keyboard);

	_gdt_init();

	_idt_init();

	_pdt_init();
}











__KERNEL__ static void _shadow_copy()
{
	screen_write_str("_shadow_copy\n");

	_memcpy(&__SHDW_ISR_DST, 		&__SHDW_ISR_ORG, 		(dword) &__SHDW_ISR_LEN);
	_memcpy(&__SHDW_GDT_DST, 		&__SHDW_GDT_ORG, 		(dword) &__SHDW_GDT_LEN);
	_memcpy(&__SHDW_SYS_TABLES_DST, &__SHDW_SYS_TABLES_ORG, (dword) &__SHDW_SYS_TABLES_LEN);
	_memcpy(&__SHDW_DATA_DST,  		&__SHDW_DATA_ORG, 		(dword) &__SHDW_DATA_LEN);
}











__KERNEL__ static void _bss_init()
{
	screen_write_str("_bss_init\n");

	dword offset;

	for(offset = 0; offset <= (dword) &__SHDW_TAREA1_BSS_LEN; offset++)
	{
		*((&__SHDW_TAREA1_BSS_PHY) + offset) = 0;
	}
}











__KERNEL__ static void _gdt_init()
{
	screen_write_str("_gdt_init\n");

	_load_gdt(gdtr);
}











__KERNEL__ static void _idt_init()
{
	screen_write_str("_idt_init\n");

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

	_load_idt(idtr);
}











__KERNEL__ static void _pdt_init()
{
	screen_write_str("_pdt_init\n");

	dword offset;


	_pdt_ptr_load();

	screen_write_str("Paginando Stack_32\n");
	for(offset = 0x00000000; offset <= ((dword) &__STACK_SIZE_32)*4; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__STACK_START_32 + offset), (&__STACK_START_32 + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	screen_write_str("Paginando ISR\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_ISR_LEN; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__SHDW_ISR_DST + offset), (&__SHDW_ISR_DST + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_R);
	}

	screen_write_str("Paginando GDT\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_GDT_LEN; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__SHDW_GDT_DST + offset), (&__SHDW_GDT_DST + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_R);
	}

	screen_write_str("Paginando Sys_tables\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_SYS_TABLES_LEN; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__SHDW_SYS_TABLES_DST + offset), (&__SHDW_SYS_TABLES_DST + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_R);
	}

	screen_write_str("Paginando Kernel_32\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_KERNEL32_LEN; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__SHDW_KERNEL32_DST + offset), (&__SHDW_KERNEL32_DST + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_R);
	}

	screen_write_str("Paginando Data\n");
	for(offset = 0x00000000; offset < (dword) &__SHDW_DATA_LEN; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__SHDW_DATA_DST + offset), (&__SHDW_DATA_DST + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	screen_write_str("Paginando Tabla_digitos\n");
	for(offset = 0x00000000; offset < (dword) &__TABLA_DIGITOS_LEN; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__TABLA_DIGITOS_LIN + offset), (&__TABLA_DIGITOS_LIN + offset), PAGE_ATTR_USER | PAGE_ATTR_RW);
	}

	screen_write_str("Paginando Keyboard_buffer\n");
	for(offset = 0x00000000; offset < (dword) &__KEYBOARD_BUFFER_LEN; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__KEYBOARD_BUFFER_LIN + offset), (&__KEYBOARD_BUFFER_LIN + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	screen_write_str("Paginando PDT\n");
	for(offset = 0x00000000; offset < (dword) &__PDT_LEN; offset += 0x1000)
	{
		_pagine(pdt, pt, (&__PDT_START + offset), (&__PDT_START + offset), PAGE_ATTR_KERNEL | PAGE_ATTR_RW);
	}

	screen_write_str("Paginando VGA\n");
	_pagine(pdt, pt, (const void *) FRAME_BUFFER, (const void *) FRAME_BUFFER, PAGE_ATTR_USER | PAGE_ATTR_RW);

	
	// TAREA 1
	screen_write_str("Paginando Tarea 1\n");
	_pagine(pdt, pt, &__SHDW_TAREA1_TEXT_PHY, &__SHDW_TAREA1_TEXT_DST, PAGE_ATTR_USER | PAGE_ATTR_R);
	_pagine(pdt, pt, &__SHDW_TAREA1_BSS_PHY,  &__SHDW_TAREA1_BSS_DST, PAGE_ATTR_USER | PAGE_ATTR_RW);
	_pagine(pdt, pt, &__SHDW_TAREA1_DATA_PHY, &__SHDW_TAREA1_DATA_DST, PAGE_ATTR_USER | PAGE_ATTR_RW);

	for(offset = 0x00000000; offset <= ((dword) &__STACK_TAREA1_SIZE)*4; offset += 0x1000)
	{
		_pagine(pdt, pt, (void *) (&__STACK_TAREA1_PHY + offset), (void *) (&__STACK_TAREA1_LIN + offset), PAGE_ATTR_USER | PAGE_ATTR_RW);
	}

	_cr3_load(pdt);

	screen_write_str("Habilitando paginacion...\n");
	_paging_enable();
	screen_reset();
}