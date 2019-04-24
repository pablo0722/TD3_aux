;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .main16

GLOBAL _init16

EXTERN __STACK_SEG_16
EXTERN __STACK_OFFSET_16
EXTERN __SHDW1_LEN
EXTERN __SHDW1_ORG
EXTERN __SHDW1_DST
EXTERN __SHDW1_JMP
EXTERN _hlt
EXTERN _TD3_memcopy

USE16










;********************************************************
;						INCLUDES						*
;********************************************************
%include "processor-flags.inc"
%include "A20G.inc"
%include "macros.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void _init16();
_init16:
	.LFB0:
		test eax, 0x0 	; Verificar que el uP no este en fallo
		jne _hlt		; Jump if Not Equal (salta si eax != 0)

		xor eax, eax 	; EAX = 0
		mov cr3, eax 	;Invalidar TLB
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










; void loadGDT();
_loadGDT:
	.LFB0:
   		;o32 lgdt  [cs:__GDTR_ADDRESS]
	.LFE0:










; void initChipset();
_initChipset:
	.LFB0:
	;   mov cx, 0x2                            ;Interrumpir cada 2mseg
	;   call PIT_Set_Counter0

	;   mov bx, 0x2820                         ;Base PIC0=0x20 PIC1=0x28
	;	call PIC_Config
	.LFE0:










; void stackInit();
_stackInit:
	.LFB0:
		; Inicializo SS y SP
		MOV_SEG ss, __STACK_SEG_16, ax
		mov sp, __STACK_OFFSET_16
	.LFE0:










; void firmwareShadow();
_firmwareShadow:
	.LFB0:
		xchg bx, bx
		CLR_SEG 	es, ax
		CALL		_TD3_memcopy, __SHDW1_DST, __SHDW1_ORG, __SHDW1_LEN, 

		; Comienzo a ejecutar desde esa ubicacion
		jmp 0x0000:__SHDW1_JMP
	.LFE0:
