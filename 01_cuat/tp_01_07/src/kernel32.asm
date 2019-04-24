;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .kernel32

GLOBAL _kernel32

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "header.inc"
%include "funciones.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void kernel32();
_kernel32:
	.LFB0:
		MBP 6
	.key_interrupts:
		call _keyboardPoll

		cmp al, KEY_Y
		jz _IntGenDE

		cmp al, KEY_U
		jz _IntGenUD

		cmp al, KEY_I
		jz _IntGenDF

		cmp al, KEY_O
		jz _IntGenGP

		cmp al, KEY_U
		jz _IntGenUD

		cmp al, KEY_P
		jz _IntGenPF

		jmp .key_interrupts
	.LFE0: