;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .isr

GLOBAL _IrqDE
GLOBAL _IrqNMI
GLOBAL _IrqBP
GLOBAL _IrqOF
GLOBAL _IrqBR
GLOBAL _IrqUD
GLOBAL _IrqNM
GLOBAL _IrqDF
GLOBAL _IrqTS
GLOBAL _IrqNP
GLOBAL _IrqSS
GLOBAL _IrqGP
GLOBAL _IrqPF
GLOBAL _IrqMF
GLOBAL _IrqAC
GLOBAL _IrqMC
GLOBAL _IrqXF

GLOBAL _IrqTM0
GLOBAL _IrqKB

EXTERN _isr_de 	; Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
EXTERN _isr_nmi
EXTERN _isr_bp
EXTERN _isr_of
EXTERN _isr_br
EXTERN _isr_ud 	; Invalid Opcode Exception
EXTERN _isr_nm
EXTERN _isr_df 	; Doble Fault
EXTERN _isr_ts
EXTERN _isr_np
EXTERN _isr_ss
EXTERN _isr_gp 	; General Protection
EXTERN _isr_pf 	; Page Fault
EXTERN _isr_mf
EXTERN _isr_ac
EXTERN _isr_mc
EXTERN _isr_xf

EXTERN _isr_tm0	; TIMER0 	irq 0 del PIC 1
EXTERN _isr_kb 	; KeyBoard 	irq 1 del PIC 1

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "x86/x86.inc"










;************************************************************
;						IRQs	                        	*
;************************************************************
_IrqDE:					; Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 0
		call _isr_de
		pop edx

		iret
	.LFE:

_IrqNMI:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 2
		call _isr_nmi
		pop edx

		iret
	.LFE:

_IrqBP:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 3
		call _isr_bp
		pop edx

		iret
	.LFE:

_IrqOF:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 4
		call _isr_of
		pop edx

		iret
	.LFE:

_IrqBR:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 5
		call _isr_br
		pop edx

		iret
	.LFE:

_IrqUD:					; Invalid Opcode Exception
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 6
		call _isr_ud
		pop edx

		iret
	.LFE:

_IrqNM:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 7
		call _isr_nm
		pop edx

		iret
	.LFE:

_IrqDF:					; Doble Fault
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 8
		call _isr_df
		pop edx

		iret
	.LFE:

_IrqTS:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 10
		call _isr_ts
		pop edx

		iret
	.LFE:

_IrqNP:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 11
		call _isr_np
		pop edx

		iret
	.LFE:

_IrqSS:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 12
		call _isr_ss
		pop edx

		iret
	.LFE:

_IrqGP:					; General Protection
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 13
		call _isr_gp
		pop edx

		iret
	.LFE:

_IrqPF:					; Page Fault
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 14
		call _isr_pf
		pop edx

		iret
	.LFE:

_IrqMF:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 16
		call _isr_mf
		pop edx

		iret
	.LFE:

_IrqAC:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 17
		call _isr_ac
		pop edx

		iret
	.LFE:

_IrqMC:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 18
		call _isr_mc
		pop edx

		iret
	.LFE:

_IrqXF:
	.LFB:
		MBP _ISR1

		push edx
		mov edx, 19
		call _isr_xf
		pop edx

		iret
	.LFE:

_IrqTM0: 				; Timer 0
	.LFB:
		MBP _ISR2

		PUSH eax

		call _isr_tm0

		mov al,20h			; Comando de acknowledge de la interrupcion actual
		out 20h,al  		; acknowledge la interrupcion del PIC 1

		POP eax

		iret
	.LFE:

_IrqKB:					; KeyBoard IRQ 1 del PIC 1
	.LFB:
		MBP _ISR2

		PUSH eax, ebx

		xor eax, eax
		in al, PORT_A_8042

		mov ebx, eax

		CALL _isr_kb, ebx

		mov al,20h			; Comando de acknowledge de la interrupcion actual
		out 20h,al  		; acknowledge la interrupcion del PIC 1

		POP eax, ebx

		iret
	.LFE: