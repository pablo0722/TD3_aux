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
extern dword __SHDW_ISR_DST;
extern dword __SHDW_ISR_ORG;
extern dword __SHDW_ISR_LEN;
extern dword __SHDW_GDT_DST;
extern dword __SHDW_GDT_ORG;
extern dword __SHDW_GDT_LEN;
extern dword __SHDW_SYS_TABLES_DST;
extern dword __SHDW_SYS_TABLES_ORG;
extern dword __SHDW_SYS_TABLES_LEN;
extern dword __SHDW_DATA_DST;
extern dword __SHDW_DATA_ORG;
extern dword __SHDW_DATA_LEN;










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					LOCAL FORWARDS				*
 ************************************************/
__KERNEL__ static void _shadow_copy();
__KERNEL__ static void _gdt_init();
__KERNEL__ static void _idt_init();










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KERNEL__ void _kernel_init()
{
	_shadow_copy();
	
	_ring_buffer_init(&ring_buffer_keyboard);

	_gdt_init();

	_idt_init();
}











__KERNEL__ static void _shadow_copy()
{
	_memcpy(&__SHDW_ISR_DST, &__SHDW_ISR_ORG, (dword) &__SHDW_ISR_LEN);
	_memcpy(&__SHDW_GDT_DST, &__SHDW_GDT_ORG, (dword) &__SHDW_GDT_LEN);
	_memcpy(&__SHDW_SYS_TABLES_DST, &__SHDW_SYS_TABLES_ORG, (dword) &__SHDW_SYS_TABLES_LEN);
	_memcpy(&__SHDW_DATA_DST,  &__SHDW_DATA_ORG, (dword) &__SHDW_DATA_LEN);
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
	_isr_load(&(idt_gates[IRQ_DF]), 	&_IrqDF, 	(dword) &CODE_SEL0, IDT_TYPE_TRAP);
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