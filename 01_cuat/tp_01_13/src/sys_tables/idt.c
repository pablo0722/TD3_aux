/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "sys_tables/idt.h"

#include "kernel/kernel.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					SYS_TABLES 					*
 ************************************************/
IdtGate idt_gates[IRQ_CANT] __SYS_TABLES__ ALIGN(8);
Idtr idtr 					__SYS_TABLES__ ALIGN(8);










/************************************************
 * 					DATA 	 					*
 ************************************************/
// BSS

// DATA










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _isr_load(IdtGate *idt_gate, void (*isr)(void), word segment, IdtType type)
{
	idt_gate->offset00_15 = 	((dword) isr) & 0xFFFF; // ISR half low
	idt_gate->segselect = 		segment; 				// Selector de segmento de código
	idt_gate->none = 			0; 						// reservado
	idt_gate->type = 			type; 					// tipo: Interrupt, Trap o Task
	idt_gate->d = 				1; 						// gate de 32 bits
	idt_gate->s = 				0; 						// Descriptor de sistema
	idt_gate->dpl = 			0;  					// Nivel de privilegio de kernel
	idt_gate->p = 				1; 						// Presente
	idt_gate->offset16_31 = 	(((dword) isr) >> 16) & 0xFFFF; 	// ISR half high
}