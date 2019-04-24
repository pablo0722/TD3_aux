/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "isr/isr.h"

#include "sys_types.h"
#include "basics.h"
#include "sys_tables/idt.h"
#include "sys_tables/pdt.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA						*
 ************************************************/










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__ISR__ void _isr_de() 	// Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
{
	MBP();
	ISR_DEFAULT(0);
}

__ISR__ void _isr_nmi()
{
	MBP();
	ISR_DEFAULT(2);
}

__ISR__ void _isr_bp()
{
	MBP();
	ISR_DEFAULT(3);
}

__ISR__ void _isr_of()
{
	MBP();
	ISR_DEFAULT(4);
}

__ISR__ void _isr_br()
{
	MBP();
	ISR_DEFAULT(5);
}

__ISR__ void _isr_ud() 	// Invalid Opcode Exception
{
	MBP();
	ISR_DEFAULT(6);
}

__ISR__ void _isr_nm()
{
	MBP();
	ISR_DEFAULT(7);
}

__ISR__ void _isr_df() 	// Doble Fault
{
	MBP();
	ISR_DEFAULT(8);
}

__ISR__ void _isr_ts()
{
	MBP();
	ISR_DEFAULT(10);
}

__ISR__ void _isr_np()
{
	MBP();
	ISR_DEFAULT(11);
}

__ISR__ void _isr_ss()
{
	MBP();
	ISR_DEFAULT(12);
}

__ISR__ void _isr_gp() 	// General Protection
{
	MBP();
	ISR_DEFAULT(13);
}

__ISR__ void _isr_pf() 	// Page Fault
{
	MBP();
	ISR_DEFAULT(14);
}

__ISR__ void _isr_mf()
{
	MBP();
	ISR_DEFAULT(16);
}

__ISR__ void _isr_ac()
{
	MBP();
	ISR_DEFAULT(17);
}

__ISR__ void _isr_mc()
{
	MBP();
	ISR_DEFAULT(18);
}

__ISR__ void _isr_xf()
{
	MBP();
	ISR_DEFAULT(19);
}

__ISR__ void _isr_kb() 	// KeyBoard isr 1 del PIC 1
{
	MBP();
	ISR_DEFAULT(33);

	/*
		MBP 2

		PUSH eax, ebx

		; in al, PORT_A_8042
		; bt eax, 0x07		; Se fija si es una tecla soltada
		; jnc .save			; Si NO es una tecla soltada, esta bien, guardo la tecla

		; xor al, al 			; si no, limpio la tecla
		; jmp .end

	.save:
		; Agrego tecla a la tabla de digitos
		; dword _ring_buffer_push(const dword *src, dword length, dword buffer_sel)
		mov [aux1], eax
		CALL _ring_buffer_push, aux1, 1, KERNEL_BUFFER_KEYBOARD

	.end:
		mov al,20h			; Comando de acknowledge de la interrupcion actual
		out 20h,al  		; acknowledge la interrupcion del PIC 1

		POP eax, ebx

		iret
	*/
}
