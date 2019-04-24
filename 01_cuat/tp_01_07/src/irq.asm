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

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "header.inc"










;************************************************************
;						IRQs	                        	*
;************************************************************
_IrqDE:					; Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
	.LFB:
		MBP 1
		mov dx, 0x0
		jmp _hlt
	.LFE:

_IrqNMI:
	.LFB:
		MBP 1
		mov dx, 0x2
		jmp _hlt
	.LFE:

_IrqBP:
	.LFB:
		MBP 1
		mov dx, 0x3
		jmp _hlt
	.LFE:

_IrqOF:
	.LFB:
		MBP 1
		mov dx, 0x4
		jmp _hlt
	.LFE:

_IrqBR:
	.LFB:
		MBP 1
		mov dx, 0x5
		jmp _hlt
	.LFE:

_IrqUD:					; Invalid Opcode Exception
	.LFB:
		MBP 1
		mov dx, 0x6
		jmp _hlt
	.LFE:

_IrqNM:
	.LFB:
		MBP 1
		mov dx, 0x7
		jmp _hlt
	.LFE:

_IrqDF:					; Doble Fault
	.LFB:
		MBP 1
		mov dx, 0x8
		jmp _hlt
	.LFE:

_IrqTS:
	.LFB:
		MBP 1
		mov dx, 0x10
		jmp _hlt
	.LFE:

_IrqNP:
	.LFB:
		MBP 1
		mov dx, 0x11
		jmp _hlt
	.LFE:

_IrqSS:
	.LFB:
		MBP 1
		mov dx, 0x12
		jmp _hlt
	.LFE:

_IrqGP:					; General Protection
	.LFB:
		MBP 1
		mov dx, 0x13
		jmp _hlt
	.LFE:

_IrqPF:					; Page Fault
	.LFB:
		MBP 1
		mov dx, 0x14
		jmp _hlt
	.LFE:

_IrqMF:
	.LFB:
		MBP 1
		mov dx, 0x16
		jmp _hlt
	.LFE:

_IrqAC:
	.LFB:
		MBP 1
		mov dx, 0x17
		jmp _hlt
	.LFE:

_IrqMC:
	.LFB:
		MBP 1
		mov dx, 0x18
		jmp _hlt
	.LFE:

_IrqXF:
	.LFB:
		MBP 1
		mov dx, 0x19
		jmp _hlt
	.LFE: