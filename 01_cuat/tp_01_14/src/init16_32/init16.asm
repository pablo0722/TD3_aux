;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .init16

GLOBAL _init16

USE16










;********************************************************
;						INCLUDES						*
;********************************************************
%include "init16_32/header16.inc"
%include "init16_32/func16.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void _init16();
_init16:
	.LFB:
		MBP _INIT16

		; Verificar buen estado del uP
		test eax, 0x0 	; Verificar que el uP no este en fallo
		je .TEST_OK		; Jump if Equal (salta si ax == 0)
		hlt
	.TEST_OK:
		; Init Stack16
		MOV_SEG ss, __STACK_SEG_16, 	ax
		MOV_SEG sp, __STACK_END_16, 	ax



		; Inicializo módulos
		call _invalide_tlb
		call _a20_enable
		call _cacheDisable
		call _timer_init
		;call _rtc_init
		call _pic_init
		call _bios_init



		; Shadow copy de la gdt

		MOV_SEG ds, __SHDW_GDT16_SEG, ax
		MOV_SEG es, __SHDW_GDT16_SEG, ax

		mov	esi,	__SHDW_GDT16_LMA ; DS:si = origen
		mov	edi,	__SHDW_GDT16_LIN ; es:di = destino
		mov	ecx,	__SHDW_GDT16_LEN ; ecx = num_bytes;

	.LLB1:		; Local Loop Begin
		mov 	bl, [CS:si]
		mov  	[ES:di], bl
		inc 	si
		inc  	di
		loopnz	.LLB1
	.LLE1: 		; Local Loop End



		; Shadow copy del init32

		MOV_SEG ds, __SHDW_INIT32_SEG, ax
		MOV_SEG es, __SHDW_INIT32_SEG, ax

		mov	si,	__SHDW_INIT32_LMA ; DS:si = origen
		mov	di,	__SHDW_INIT32_LIN ; es:di = destino
		mov	cx,	__SHDW_INIT32_LEN ; ecx = num_bytes;

	.LLB0:		; Local Loop Begin
		mov 	bl, [CS:si]
		mov  	[ES:di], bl
		inc 	si
		inc  	di
		loopnz	.LLB0
	.LLE0: 		; Local Loop End



		; Inicializa GDT

		MBP _GDT
		MOV_SEG es, 0x0000, ax
		o32 lgdt  [cs:gdtr16]

		;->Establecer el up en MP<-
		smsw ax
		or	 ax, X86_CR0_PE
		lmsw ax

		; No es necesario limpiar el prefetch queue ya que el siguiente far jump mismo lo hace
		; jmp   .flush_prefetch_queue 	; No es necesario
		; .flush_prefetch_queue: 		; No es necesario

		;	PASA A MODO PROTEGIDO
		o32 jmp dword CODE_SEL0:_init32
	.LFE: