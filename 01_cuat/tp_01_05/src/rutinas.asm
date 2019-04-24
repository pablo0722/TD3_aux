;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .rutinas

GLOBAL _A20Enable
GLOBAL _TD3_memcopy
GLOBAL _loadGDT

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "A20G.inc"
%include "macros.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
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

		mov di, .return
	.empty_8042_in:  
		;      in al, CTRL_PORT_8042      ; Lee port de estado del 8042 hasta que el
		;      test al, 00000010b         ; buffer de entrada este vacio
		;      jne .empty_8042_in
		jmp di
	.return:  
		ret
	.LFE0:










; void *TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes);
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