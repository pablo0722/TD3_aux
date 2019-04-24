/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "sys_types.h"
#include "basics.h"
#include "sys_tables/idt.h"
#include "sys_tables/pdt.h"
#include "isr/isr.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA						*
 ************************************************/










extern const dword CODE_SEL0;
extern const dword _IrqDE;
//#define CODE_SEL0 8
/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__INIT16__ void _idt_init()
{
	MBP();
	int a;
	a = sizeof(IdtGate)*IRQ_CANT-1;
	a = (int)idt_gates;
	_irq_load(&(idt_gates[IRQ_DE]), 	&_isr_de, &CODE_SEL0, IDT_TYPE_TRAP);
	// _irq_load(&(idt_gates[IRQ1]), 		NULL, 0, 0);	
	_irq_load(&(idt_gates[IRQ_NMI]), 	&_isr_nmi, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_BP]), 	&_isr_bp, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_OF]), 	&_isr_of, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_BR]), 	&_isr_br, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_UD]), 	&_isr_ud, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_NM]), 	&_isr_nm, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_DF]), 	&_isr_df, &CODE_SEL0, IDT_TYPE_TRAP);
	// _irq_load(&(idt_gates[IRQ9]), 		NULL, 0, 0);
	_irq_load(&(idt_gates[IRQ_TS]), 	&_isr_ts, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_NP]), 	&_isr_np, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_SS]), 	&_isr_ss, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_GP]), 	&_isr_gp, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_PF]), 	&_isr_pf, &CODE_SEL0, IDT_TYPE_TRAP);
	// _irq_load(&(idt_gates[IRQ15]), 		NULL, 0, 0);
	_irq_load(&(idt_gates[IRQ_MF]), 	&_isr_mf, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_AC]), 	&_isr_ac, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_MC]), 	&_isr_mc, &CODE_SEL0, IDT_TYPE_TRAP);
	_irq_load(&(idt_gates[IRQ_XF]), 	&_isr_xf, &CODE_SEL0, IDT_TYPE_TRAP);
	// _irq_load(&(idt_gates[IRQ20]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ21]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ22]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ23]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ24]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ25]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ26]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ27]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ28]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ29]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ30]), 		NULL, 0, 0);
	// _irq_load(&(idt_gates[IRQ31]), 		NULL, 0, 0);

	MBP();
	idtr.length = IRQ_CANT * sizeof(IdtGate) - 1;
	idtr.addr = idt_gates;
}

__KERNEL__ void _kernel_main()
{
}