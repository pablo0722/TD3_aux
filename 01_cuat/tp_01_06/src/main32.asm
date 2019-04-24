;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .main32

GLOBAL _main32

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "header.inc"
%include "funciones.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void main32();
_main32:
	.LFB0:

	.resetTable:
		XCHG bx, bx
		mov ecx, __DIGIT_TABLE_SIZE_B
		mov edi, __DIGIT_TABLE_START

	.readTeclado:
		call _keyboardPoll

		cmp al, 0x1F
		jz .stopPolling

		cmp al, 0x0B		; if al <= '0'
		jng .menorCero
		jmp .readTeclado

	.menorCero:
		cmp al, 0x02		; if al >= '1'
		jge .numero			; entonces es un numero. Tengo que pushearlo
		jmp .readTeclado

	.numero:
		mov [edi], al
		inc edi
		dec ecx
		jz	.resetTable
		jmp .readTeclado

	.stopPolling:
		XCHG bx, bx
		jmp _hlt

	.LFE0: