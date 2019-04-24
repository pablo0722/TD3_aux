;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .init32

GLOBAL _init32

EXTERN __STACK_SIZE_32_DW
EXTERN __STACK_END_32
EXTERN __SHDW_CODE_LEN
EXTERN __SHDW_CODE_ORG
EXTERN __SHDW_CODE_DST
EXTERN __SHDW_CODE_JMP
EXTERN _A20Enable
EXTERN _TD3_memcopy
EXTERN CODE_SEL0
EXTERN DATA_SEL0

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "macros.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void _init32();
_init32:
	.LFB0:
		XCHG bx, bx

		call _A20Enable

		;->Inicializar selector de pila   
		MOV_SEG ss, DATA_SEL0, ax
		mov esp, __STACK_END_32

	%if DEBUG != 1
		;->Inicializar la pila   
		xor ebx, ebx
		FOR __STACK_SIZE_32_DW, \
		{ \
			push ebx \
		}
		mov esp, __STACK_END_32
	%endif

		;->Inicializar los selectores de datos
		mov ds, ax
		mov es, ax
		mov gs, ax
		mov fs, ax

		;->Desempaquetar la ROM

		XCHG 	bx, bx
		
		; shadow copy de codigo
		CALL	_TD3_memcopy, __SHDW_CODE_DST, __SHDW_CODE_ORG, __SHDW_CODE_LEN

		XCHG bx, bx
		; Comienzo a ejecutar desde esa ubicacion
		jmp CODE_SEL0:__SHDW_CODE_JMP
	.LFE0: