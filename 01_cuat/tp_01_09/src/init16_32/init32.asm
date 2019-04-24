;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .init32

GLOBAL _init32

EXTERN _kernel_main

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "x86/x86.inc"










;********************************************************
;						DEFINES LINKER					*
;********************************************************
EXTERN __STACK_SIZE_32
EXTERN __STACK_END_32

EXTERN __SHDW_KERNEL32_LEN
EXTERN __SHDW_KERNEL32_ORG
EXTERN __SHDW_KERNEL32_DST

EXTERN DATA_SEL0










;********************************************************
;						FUNCIONES						*
;********************************************************
; void _init32();
_init32:
	.LFB0:
		MBP _INIT32

		;->Inicializar selector de pila   
		MOV_SEG ss, DATA_SEL0, ax
		mov esp, __STACK_END_32

	%if DEBUG != 1
		;->Inicializar la pila   
		xor ebx, ebx
		FOR __STACK_SIZE_32_DW, 	\
		{ 							\
			push ebx 				\
		}
		mov esp, __STACK_END_32
	%endif

		;->Inicializar los selectores de datos
		mov ds, ax
		mov es, ax
		mov gs, ax
		mov fs, ax



		; Shadow copy del init32

		mov	esi,	__SHDW_KERNEL32_ORG ; DS:si = origen
		mov	edi,	__SHDW_KERNEL32_DST ; es:di = destino
		mov	ecx,	__SHDW_KERNEL32_LEN ; ecx = num_bytes;

	.LLB:		; Local Loop Begin
		mov 	bl, [esi]
		mov  	[edi], bl
		inc 	si
		inc  	di
		loopnz	.LLB
	.LLE: 		; Local Loop End

		xor eax, eax
		; Comienzo a ejecutar desde el main del kernel
		jmp _kernel_main
	.LFE0:
