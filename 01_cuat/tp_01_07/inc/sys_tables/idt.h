/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "sys_types.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					ENUM						*
 ************************************************/
typedef enum
{
	IDT_TYPE_TASK = 5, 	// Task Gate
	IDT_TYPE_INT = 	6, 	// Interrupt Gate
	IDT_TYPE_TRAP = 7, 	// Trap Gate
} IdtType;

typedef enum
{
	IRQ_DE, 	// Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
	IRQ1, 		// 
	IRQ_NMI, 	// 
	IRQ_BP, 	// 
	IRQ_OF, 	// 
	IRQ_BR, 	// 
	IRQ_UD, 	// Invalid Opcode Exception
	IRQ_NM, 	// 
	IRQ_DF, 	// Doble Fault
	IRQ9, 		// 
	IRQ_TS, 	// 
	IRQ_NP, 	// 
	IRQ_SS, 	// 
	IRQ_GP, 	// General Protection
	IRQ_PF, 	// Page Fault
	IRQ15, 		// 
	IRQ_MF, 	// 
	IRQ_AC, 	// 
	IRQ_MC, 	// 
	IRQ_XF, 	// 

	// Reservados
	IRQ20, 		// 
	IRQ21, 		// 
	IRQ22, 		// 
	IRQ23, 		// 
	IRQ24, 		// 
	IRQ25, 		// 
	IRQ26, 		// 
	IRQ27, 		// 
	IRQ28, 		// 
	IRQ29, 		// 
	IRQ30, 		// 
	IRQ31, 		// 

	// Disponible para el usuario
	IRQ32, 		// 
	IRQ33, 		// 
	IRQ34, 		// 
	IRQ35, 		// 
	IRQ36, 		// 
	IRQ37, 		// 
	IRQ38, 		// 
	IRQ39, 		// 
	IRQ40, 		// 
	IRQ41, 		// 
	IRQ42, 		// 
	IRQ43, 		// 
	IRQ44, 		// 
	IRQ45, 		// 
	IRQ46, 		// 
	IRQ47, 		// 

	IRQ_CANT
} IrqIdx;










/************************************************
 * 					ESTRUCTURAS					*
 ************************************************/
typedef struct
{
	qword offset00_15: 	16;	// Offset15_00
	qword segselect: 	16;	// Selector de segmento
	qword none: 		8;	// 0[7] - 0[6] -     0[5] - reservado[4:0]
	qword type: 		3; 	// "110"
	qword d: 			1;  // D[3]
	qword s: 			1;  // S[4]
	qword dpl: 			2;  // DPL[6:5]
	qword p: 			1;  // P[7]
	qword offset16_31: 	16;	// Offset31_16
} IdtGate; // IDT (Interrupt / Trap / Task) Gate

typedef struct PACKED
{
	word length; 	// Tamaño de la tabla en reg 16 bits
	dword addr; 	// Dirección relativa en reg 32 bits
} Idtr; // IDT register










/************************************************
 * 					SYS_TABLES 					*
 ************************************************/
extern IdtGate 	idt_gates[IRQ_CANT] __SYS_TABLES__ ALIGN(64);
extern Idtr 	idtr 				__SYS_TABLES__ ALIGN(64);










/************************************************
 * 					DATA 	 					*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__FUNCTION__ void _irq_load(IdtGate *idt_gate, void (*isr)(void), word segment, IdtType type);