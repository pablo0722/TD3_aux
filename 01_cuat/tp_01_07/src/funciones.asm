;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .funciones

GLOBAL _PIC_Config

GLOBAL _TD3_memcopy
GLOBAL _keyboardPoll
GLOBAL _load_cr3

GLOBAL _IntGenDE
GLOBAL _IntGenNMI
GLOBAL _IntGenBP
GLOBAL _IntGenOF
GLOBAL _IntGenBR
GLOBAL _IntGenUD
GLOBAL _IntGenNM
GLOBAL _IntGenDF
GLOBAL _IntGenTS
GLOBAL _IntGenNP
GLOBAL _IntGenSS
GLOBAL _IntGenGP
GLOBAL _IntGenPF
GLOBAL _IntGenMF
GLOBAL _IntGenAC
GLOBAL _IntGenMC
GLOBAL _IntGenXF

; funciones en C
EXTERN _cr3_init 	; void _cr3_init()
EXTERN _pde_init 	; void _pde_init()
EXTERN _pte_init 	; void _pte_init()

EXTERN pde

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "header.inc"










;************************************************************
;						INICIALIZACION	                        	*
;************************************************************
USE16
_PIC_Config:                     ; Inicializo PIC 1
    mov     al, 11h             ; ICW1: IRQs activas x flanco, cascada, e ICW4
    out     20h, al  
    mov     al, bl              ; ICW2: El PIC 1 arranca en INT tipo "bl"
    out     21h, al
    mov     al, 04h             ; ICW3: PIC 1 Master, Slave, Ingresa Int x IRQ2
    out     21h,al
    mov     al, 01h             ; ICW4: Modo 8086
    out     21h, al
    mov     al, 0FFh            ; Deshabilito las interrupciones del PIC 1 
    out     21h, al
    mov     al, 11h             ; ICW1: IRQs activas x flanco, cascada, e ICW4
    out     0A0h, al  
    mov     al, bh              ; ICW 2: El PIC 2 arranca en INT tipo "bh"
    out     0A1h, al
    mov     al, 02h             ; ICW 3: PIC 2 Slave, Ingresa Int x IRQ2
    out     0A1h, al
    mov     al, 01h             ; ICW 4: Modo 8086
    out     0A1h, al
    mov     al, 0FFh            ; Deshabilito las interrupciones del PIC 2
    out     0A1h, al

    mov 	al, 0xff			; Deshabilita todas las interrupciones del PIC 1
    out 	0x21, al			

    mov 	al, 0xff			; Deshabilita todas las interrupciones del PIC 2
    out 	0xA1, al			

    ret
USE32










;************************************************************
;						IRQs	                        	*
;************************************************************
; void *_TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes);
; Copia <num_bytes> bytes de la posicion <origen> en <destino>
_TD3_memcopy:
	.LFB0:					; // Local Function Begin: etiqueta local, comienzo de funcion
		push ebp			; // Guarda el Base Pointer (porque despues se va a pisar)
		mov	 ebp, esp		; ebp = esp;

	;	mov 		esp, -N 	; Reservo variables locales

		push edi
		push esi
		push ecx
	.LFB1:					; Comienza codigo
							; [ebp]: antiguo ebp, [ebp+4]: antiguo IP (ocupa 4 bytes en 32 bits), 
							; [ebp+8]: primer argumento de la izquierda de la funcion, [ebp+12]: segundo argumento, etc
							; [ebp-4]: primer variable local, [ebp-8]: segunda variable local, etc.
		mov	eax,	[ebp+8]	; valor de return = destino;
		mov	edi,	eax		; es:di = destino
		mov	esi,	[ebp+12]	; DS:si = origen
		mov	ecx,	[ebp+16]	; ecx = num_bytes;
	.LLB0:					; Local Loop Begin
		mov 	bl, [CS:esi]
		mov  	[ES:edi], bl
		inc 	esi
		inc  	edi
		loopnz	.LLB0
	.LLE0:					; Local Loop End
	.LFE1:					; Termina codigo
		pop	ecx
		pop	esi
		pop	edi

	;	mov			SP, BP	; SP = BP; Elimina las variables locales (en este ejemplo no hay)
		
		pop	ebp				; // Restaura el Base Pointer
		ret 				; Return;
	.LFE0:					; // Local Function End: etiqueta local, fin de funcion










; void _keyboardPoll();
_keyboardPoll:
	.kybrd_poll:
		xor eax, eax
		in al, CTRL_PORT_8042
		bt eax, 0x00
		jnc .kybrd_poll

		in al, PORT_A_8042
		bt eax, 0x07		; Se fija si es una tecla soltada
		jc .kybrd_poll		; Si es una tecla soltada, vuelve a buscar otra tecla
		ret










; void _load_cr3(dword cr3);
_load_cr3:
	.LFB:
		mov eax, [esp+4]
		mov cr3, eax
		ret
	.LFE:










_IntGenDE:
	.LFB:
		MBP 7
		mov bx, 0x00
		mov ax, 0x00
		div bx
	.LFE:

_IntGenNMI:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenBP:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenOF:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenBR:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenUD:
	.LFB:
		LOCK mov ax, bx		; Hacer un MOV con el prefijo LOCK genera un #UD
	.LFE:

_IntGenNM:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenDF:
	.LFB:
		; Para hacer Double Fault, se debe generar una interrupcion en la vectorización de la primer interrupcion
		MBP 7
		mov esp, 0x0
		mov bx, 0x00
		mov ax, 0x00
		div bx
	.LFE:

_IntGenTS:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenNP:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenSS:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenGP:
	.LFB:
		MBP 7
		mov eax, 0xffffffff
		mov ss, eax
	.LFE:

_IntGenPF:
	.LFB:
		MBP 7

		; call _pte_init
		; call _pde_init
		; call _cr3_init

		MBP 7

		; Habilita paginacion
		mov esp, 0x0
		mov eax, cr0
		or	eax, 0x80000000
		mov cr0, eax

		nop
		nop
		nop

		jmp $
	.LFE:

_IntGenMF:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenAC:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenMC:
	.LFB:
		jmp _hlt
	.LFE:

_IntGenXF:
	.LFB:
		jmp _hlt
	.LFE: