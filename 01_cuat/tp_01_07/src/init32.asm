;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .init32

GLOBAL _init32

EXTERN _kernel32

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "header.inc"
%include "funciones.inc"










EXTERN _kernel_main
;********************************************************
;						FUNCIONES						*
;********************************************************
; void _init32();
_init32:
	.LFB0:
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

		;->Desempaquetar la ROM

		MBP 6

		call _PIC_Config
		
		; shadow copy de codigo
		CALL	_TD3_memcopy, __SHDW_KERNEL32_DST, __SHDW_KERNEL32_ORG, __SHDW_KERNEL32_LEN
		
		; shadow copy de sys_tables
		CALL	_TD3_memcopy, __SHDW_SYS_TABLES_DST, __SHDW_SYS_TABLES_ORG, __SHDW_SYS_TABLES_LEN

		;->Inicializar los selectores de datos
		call _idt_init
		MBP 7
		lidt [idtr]
		sti

		MBP 6
		; Comienzo a ejecutar desde esa ubicacion
		jmp _kernel32
	.LFE0: