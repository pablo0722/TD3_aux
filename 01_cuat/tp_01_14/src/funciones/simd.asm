;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .funciones

GLOBAL _simd_suma_desborde_byte
GLOBAL _simd_suma_desborde_dword

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "x86/x86.inc"










;************************************************************
;						INFORMACION		                   	*
;************************************************************
; Hay 8 registros XMM0-7 de 128 bits cada uno
;
; Habilitar instrucciones SSE:
; 	clear the 	CR0.EM 			If set, no x87 floating point unit present, if clear, x87 FPU present
; 	set the 	CR0.MP 			Controls interaction of WAIT/FWAIT instructions with TS flag in CR0
; 	set the 	CR0.TS 			Allows saving x87 task context upon a task switch only after x87 instruction used
; 	set the 	CR4.OSFXSR 		If set, enables SSE instructions and fast FPU save & restore
; 	set the 	CR4.OSXMMEXCPT 	If set, enables unmasked SSE exceptions.











;************************************************************
;						FUNCIONES                        	*
;************************************************************
_simd_suma_desborde_byte:
	.LFB:
		PUSH eax, ecx

		mov 	eax, 	[esp+12]
		mov 	ecx, 	[esp+20]

	.for_begin:
		movdqa 	xmm0, 	[eax]
		jmp 	.for_cmp

	.for_do:
		add 	eax, 	0x10
		movdqa 	xmm1, 	[eax]

		paddb 	xmm0, 	xmm1
		sub ecx, 4

	.for_cmp:
		cmp ecx, 0x04
		jg 	.for_do

	.for_end:

		phaddd 		xmm0, 	xmm0
		phaddd 		xmm0, 	xmm0

		mov 		eax, 	[esp+16]
		movd 		[eax], 	xmm0
		and dword	[eax], 	0x000000FF

		POP eax, ecx



		ret
	.LFE:










_simd_suma_desborde_dword:
	.LFB:
		PUSH eax, ecx

		mov 	eax, 	[esp+12]
		mov 	ecx, 	[esp+20]

	.for_begin:
		movdqa 	xmm0, 	[eax]
		jmp 	.for_cmp

	.for_do:
		add 	eax, 	0x10
		movdqa 	xmm1, 	[eax]

		phaddd 	xmm0, 	xmm1
		sub ecx, 4

	.for_cmp:
		cmp ecx, 0x04
		jg 	.for_do

	.for_end:

		phaddd 	xmm0, 	xmm0
		phaddd 	xmm0, 	xmm0

		mov 	eax, 	[esp+16]
		movd 	[eax], 	xmm0

		POP eax, ecx



		ret
	.LFE: