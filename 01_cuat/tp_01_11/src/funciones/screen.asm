;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .funciones

GLOBAL _screen_clean

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "x86/x86.inc"










;************************************************************
;						FUNCIONES                        	*
;************************************************************
_screen_clean:
	.LFB:
		PUSH ebx, ecx, edi

		xor ebx, ebx

	.col:
		xor edi, edi
		mov ecx, [esp+16]

	.row:
		mov byte [ebx + edi + 0x000B8000], 'H'
		add edi, 1
		mov byte [ebx + edi + 0x000B8000], 0x1F
		add edi, 1
		loop .row
		add ebx, 160
		cmp ebx, 0x1000
		jle .col

		POP ebx, ecx, edi

		ret
	.LFE:
