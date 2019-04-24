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
EXTERN __SHDW_KERNEL32_LMA
EXTERN __SHDW_KERNEL32_LIN

EXTERN __SHDW_KFUNC32_LEN
EXTERN __SHDW_KFUNC32_LMA
EXTERN __SHDW_KFUNC32_LIN

EXTERN __SHDW_FUNC32_LEN
EXTERN __SHDW_FUNC32_LMA
EXTERN __SHDW_FUNC32_LIN

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

		mov	esi,	__SHDW_KERNEL32_LMA ; DS:si = origen
		mov	edi,	__SHDW_KERNEL32_LIN ; es:di = destino
		mov	ecx,	__SHDW_KERNEL32_LEN ; ecx = num_bytes;

	.LLB0:		; Local Loop Begin
		mov 	bl, [esi]
		mov  	[edi], bl
		inc 	si
		inc  	di
		loopnz	.LLB0
	.LLE0: 		; Local Loop End



		; Shadow copy del func32

		mov	esi,	__SHDW_KFUNC32_LMA ; DS:si = origen
		mov	edi,	__SHDW_KFUNC32_LIN ; es:di = destino
		mov	ecx,	__SHDW_KFUNC32_LEN ; ecx = num_bytes;

	.LLB1:		; Local Loop Begin
		mov 	bl, [esi]
		mov  	[edi], bl
		inc 	si
		inc  	di
		loopnz	.LLB1
	.LLE1: 		; Local Loop End



		; Shadow copy del func32

		mov	esi,	__SHDW_FUNC32_LMA ; DS:si = origen
		mov	edi,	__SHDW_FUNC32_LIN ; es:di = destino
		mov	ecx,	__SHDW_FUNC32_LEN ; ecx = num_bytes;

	.LLB2:		; Local Loop Begin
		mov 	bl, [esi]
		mov  	[edi], bl
		inc 	si
		inc  	di
		loopnz	.LLB2
	.LLE2: 		; Local Loop End



		mov eax, 0x1
		cpuid
		test edx, 1<<25
		jz .noSSE

		; Habilitar instrucciones SSE:
		; 	clear the 	CR0.EM 			If set, no x87 floating point unit present, if clear, x87 FPU present
		; 	set the 	CR0.MP 			Controls interaction of WAIT/FWAIT instructions with TS flag in CR0
		; 	set the 	CR0.TS 			Allows saving x87 task context upon a task switch only after x87 instruction used
		; 	set the 	CR4.OSFXSR 		If set, enables SSE instructions and fast FPU save & restore
		; 	set the 	CR4.OSXMMEXCPT 	If set, enables unmasked SSE exceptions.
		; 	set the 	CR4.OSXSAVE 	If set, enables XSAVE/XRSTOR instruction.

		; 	clear the 	CR0.EM (emulation) 				bit (bit 2) 	[ CR0 &= ~(1 << 2) ]
		; 	set the 	CR0.MP (monitor co-processor) 	bit (bit 1) 	[ CR0 |= (1 << 1) ]
		; 	set the 	CR0.TS (Task switched) 			bit (bit 3) 	[ CR0 |= (1 << 3) ]
		mov eax, 	cr0
		and ax, 	0xFFFB
		;or  ax, 	0x1 << 1
		or  ax, 	0x1 << 3
		mov cr0, 	eax

		; 	set the CR4.OSFXSR 		(os support for fxsave and fxrstor instructions) 			bit (bit 9) 	[ CR4 |= (1 << 9) ]
		; 	set the CR4.OSXMMEXCPT 	(os support for unmasked simd floating point exceptions) 	bit (bit 10) 	[ CR4 |= (1 << 10) ]
		; 	set the CR4.OSXSAVE 	(XSAVE and Processor Extended States Enable) 				bit (bit 18) 	[ CR4 |= (1 << 10) ]
		mov eax, 	cr4
		or 	eax, 	0x1 << 9
		;or 	eax, 	0x1 << 10
		;or 	eax, 	0x1 << 18
		mov cr4, 	eax


		jmp .siSSE

	.noSSE:
		; paddb xmm1, xmm2 Genera Excepcion 6 (#UD = Invalid Opcode)
		
		hlt
		jmp .noSSE

	.siSSE:



		xor eax, eax
		; Comienzo a ejecutar desde el main del kernel
		jmp _kernel_main
	.LFE0:
