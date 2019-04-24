;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .init16

GLOBAL _init16

EXTERN _init32

USE16










;********************************************************
;						INCLUDES						*
;********************************************************
%include "header.inc"
%include "funciones.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void _init16();
_init16:
	.LFB0:
		; Verificar buen estado del uP
		test eax, 0x0 	; Verificar que el uP no este en fallo
		je .TEST_OK		; Jump if Equal (salta si ax == 0)
		jmp 0x0000:_hlt		; Jump if Not Equal (salta si ax != 0)
	.TEST_OK:
		xor eax, eax 		; ax = 0
		mov cr3, eax 	;Invalidar TLB

		; Inicializo SS y SP
		MOV_SEG ss, __STACK_SEG_16, ax
		mov sp, __STACK_OFFSET_16

		XCHG 	bx, bx

		;Shadow Copy Routines

		CLR_SEG es, ax

		; Shadow copy de _hlt
		mov	di,		__SHDW_HALT_DST ; es:di = destino
		mov	si,		__SHDW_HALT_ORG	; DS:si = origen
		mov	ecx,	__SHDW_HALT_LEN	; ecx = num_bytes;
	.LLB0:					; Local Loop Begin
		mov 	bl, [CS:si]
		mov  	[ES:di], bl
		inc 	si
		inc  	di
		loopnz	.LLB0

		; Shadow copy de rutinas de inicializacion
		mov	di,		__SHDW_RUTN_DST ; es:di = destino
		mov	si,		__SHDW_RUTN_ORG	; DS:si = origen
		mov	ecx,	__SHDW_RUTN_LEN	; ecx = num_bytes;
	.LLB1:					; Local Loop Begin
		mov 	bl, [CS:si]
		mov  	[ES:di], bl
		inc 	si
		inc  	di
		loopnz	.LLB1
	.LFE0:










; void cacheDisable();
_cacheDisable:
	.LFB0:
	;	mov   eax, cr0
	;	or    eax, (X86_CR0_NW | X86_CR0_CD)
	;	mov   cr0, eax
	;	wbinvd
	.LFE0:










; void initChipset();
_initChipset:
	.LFB0:
	;   mov cx, 0x2                            ;Interrumpir cada 2mseg
	;   call PIT_Set_Counter0

	;   mov bx, 0x2820                         ;Base PIC0=0x20 PIC1=0x28
	;	call PIC_Config
	.LFE0:










; void loadGDT();
_loadGDT:
	.LFB0:
		XCHG bx, bx
		o32 lgdt  [cs:rom_gdtr]

		;->Establecer el up en MP<-
		smsw  ax
		or	 ax, X86_CR0_PE
		lmsw  ax

		; No es necesario limpiar el prefetch queue ya que el siguiente far jump mismo lo hace
		;jmp   .flush_prefetch_queue
		;.flush_prefetch_queue:

		;	PASA A MODO PROTEGIDO
		o32 jmp dword CODE_SEL0:_init32
	.LFE0: