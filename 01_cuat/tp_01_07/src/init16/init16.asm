;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .init16

GLOBAL _init16
GLOBAL _A20Enable

EXTERN _idt_init16
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
		hlt
	.TEST_OK:
		xor eax, eax 	; ax = 0
		mov cr3, eax 	;Invalidar TLB

		MBP 6

		;Shadow Copy Routines

		CLR_SEG es, ax

		; Shadow copy de rutinas y handlers
		mov	di,		__SHDW_HANDLERS_DST ; es:di = destino
		mov	si,		__SHDW_HANDLERS_ORG	; DS:si = origen
		mov	ecx,	__SHDW_HANDLERS_LEN	; ecx = num_bytes;
	.LLB0:					; Local Loop Begin
		mov 	bl, [CS:si]
		mov  	[ES:di], bl
		inc 	si
		inc  	di
		loopnz	.LLB0
	.LFE0:










; void A20Enable();
_A20Enable:
	.LBF0:			; Habilita el A20 gate sin utilizacion del stack.
		;Deshabilita el teclado
		mov di, .8042_kbrd_dis
		jmp .empty_8042_in
	.8042_kbrd_dis:
		mov al, KEYB_DIS
		out CTRL_PORT_8042, al

		;Lee la salida
		mov di, .8042_read_out
		jmp .empty_8042_in
	.8042_read_out:
		mov al, READ_OUT_8042
		out CTRL_PORT_8042, al

	.empty_8042_out:  
		;      in al, CTRL_PORT_8042      ; Lee port de estado del 8042 hasta que el
		;      test al, 00000001b         ; buffer de salida este vacio
		;      jne .empty_8042_out

		xor bx, bx   
		in al, PORT_A_8042
		mov bx, ax

		;Modifica el valor del A20
		mov di, .8042_write_out
		jmp .empty_8042_in
	.8042_write_out:
		mov al, WRITE_OUT_8042
		out CTRL_PORT_8042, al

		mov di, .8042_set_a20
		jmp .empty_8042_in
	.8042_set_a20:
		mov ax, bx
		or ax, 00000010b              ; Habilita el bit A20
		out PORT_A_8042, al

		;Habilita el teclado
		mov di, .8042_kbrd_en
		jmp .empty_8042_in
	.8042_kbrd_en:
		mov al, KEYB_EN
		out CTRL_PORT_8042, al

		mov di, .LFE0
	.empty_8042_in:  
		;      in al, CTRL_PORT_8042      ; Lee port de estado del 8042 hasta que el
		;      test al, 00000010b         ; buffer de entrada este vacio
		;      jne .empty_8042_in
		jmp di
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
		MBP 6
		o32 lgdt  [cs:_gdtr]

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