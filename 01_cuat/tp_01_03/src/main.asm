; Compilo con: nasm rom.s -o rom.bin -l rom.lst
; *.s es el codigo en asembler
; *.bin es el binario. Se puede leer con: hexdump *.bin
; *.lst lista las instrucciones en el formato: fila, direccion, opcode y mnemonico. Se puede leer con: cat *.lst

; El A20 esta deshabilitado para este ejericio
; Por ahora podemos usar la RAM sin habilitarla ("bug" del bochs)









;********************************************************
;						INCLUDES						*
;********************************************************
%include "../inc/processor-flags.h"
%include "../inc/A20G.h"










;********************************************************
;						DEFINES							*
;********************************************************
; Definiciones de la ROM
%define ROM_SIZE			(64*1024)
%define ROM_POS_START		0xf0000
%define ROM_POS_RESET		0xffff0

; Tamaño del codigo util (sin contar la funcion _start)
CODE_START equ ($)
%define CODE_LEN (CODE_END - CODE_START)










;********************************************************
;						DIRECTIVAS						*
;********************************************************
section .text
use16							; use16: Por default se usa 16-bits encoding para las instrucciones; mientras que para las instrucciones 32-bits encoding los operandos deben ser prefijados (la instruccion ocupa 2 bytes) permitiendo usar registros de 32-bits

global _start










;********************************************************
;						FUNCIONES						*
;********************************************************

; void start16();
_start16:
	.LBF0:
		xor EAX, EAX 	; EAX = 0
		mov CR3, EAX 	;Invalidar TLB

	.A20_begin:			; Habilita el A20 gate sin utilizacion del stack.

		;Deshabilita el teclado
		mov dword EDI, .8042_kbrd_dis
		jmp .empty_8042_in
	.8042_kbrd_dis:
		mov AL, KEYB_DIS
		out CTRL_PORT_8042, AL

		;Lee la salida
		mov dword EDI, .8042_read_out
		jmp .empty_8042_in
	.8042_read_out:
		mov AL, READ_OUT_8042
		out CTRL_PORT_8042, AL

	.empty_8042_out:  
		;      in al, CTRL_PORT_8042      ; Lee port de estado del 8042 hasta que el
		;      test al, 00000001b         ; buffer de salida este vacio
		;      jne .empty_8042_out

		xor BX, BX   
		in AL, PORT_A_8042
		mov BX, AX

		;Modifica el valor del A20
		mov dword EDI, .8042_write_out
		jmp .empty_8042_in
	.8042_write_out:
		mov AL, WRITE_OUT_8042
		out CTRL_PORT_8042, AL

		mov dword EDI, .8042_set_a20
		jmp .empty_8042_in
	.8042_set_a20:
		mov AX, BX
		or AX, 00000010b              ; Habilita el bit A20
		out PORT_A_8042, AL

		;Habilita el teclado
		mov dword EDI, .8042_kbrd_en
		jmp .empty_8042_in
	.8042_kbrd_en:
		mov AL, KEYB_EN
		out CTRL_PORT_8042, AL

		mov dword EDI, .A20_end
	.empty_8042_in:  
		;      in al, CTRL_PORT_8042      ; Lee port de estado del 8042 hasta que el
		;      test al, 00000010b         ; buffer de entrada este vacio
		;      jne .empty_8042_in
		jmp EDI

	.A20_end:			; Habilita el A20 gate sin utilizacion del stack.

		jmp _firmware_shadow
	.LFE0:
	START16_LEN equ ($ - _start16)









; void firmware_shadow();
_firmware_shadow:
	.LFB0:
		; LLamo a la funcion void *TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes)
		push dword	(CODE_END - _main) 	; unsigned int num_bytes
		push dword 	_main 				; const void *origen
		push dword	0x00000000 			; void *destino
		call 		_TD3_memcopy		; void *TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes)
		add	 SP, 12						; Borro los 3 argumentos del STACK

		jmp _main
	.LFE0:
	FIRMWARE_SHADOW_LEN equ ($ - _firmware_shadow)










; void main();
_main:
		jmp _hlt
	MAIN_LEN equ ($ - _main)










; void *TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes);
; Copia <num_bytes> bytes de la posicion <origen> en <destino>
_TD3_memcopy:
	.LFB0:							; // Local Function Begin: etiqueta local, comienzo de funcion
		push dword 	EBP				; // Guarda el Base Pointer (porque despues se va a pisar)
		mov	 dword	EBP, ESP		; BP = SP;

	;	mov 		SP, -N 			; Reservo variables locales

		push dword	EDI
		push dword	ESI
		push dword	ECX
	.LFB1:							; Comienza codigo
		xor AX, AX 					; DS = 0;
		mov DS, AX 					; DS = 0;
		mov ES, AX 					; ES = 0;
									; [EBP]: antiguo EBP, [EBP+4]: antiguo IP (ocupa 2 bytes en call near), 
									; [EBP+6]: primer argumento de la izquierda de la funcion, [EBP+10]: segundo argumento, etc
									; [EBP-4]: primer variable local, [EBP-8]: segunda variable local, etc.
		mov dword	EAX,	[BP+6]	; valor de return = destino;
		mov dword	EDI,	EAX		; ES:DI = destino
		mov dword	ESI,	[BP+10]	; DS:SI = origen
		mov	dword	ECX,	[BP+14]	; ECX = num_bytes;
		repnz		movsb			; repnz: repite instruccion <ECX> veces
									; movsb: [ES:EDI++] = [DS:ESI++]; // (incrementa despues)
	.LFE1:							; Termina codigo
		pop dword	ECX
		pop	dword	ESI
		pop	dword	EDI

	;	mov			SP, BP			; SP = BP; Elimina las variables locales (en este ejemplo no hay)
		
		pop dword	EBP				; // Restaura el Base Pointer
		ret 						; Return;
	.LFE0:							; // Local Function End: etiqueta local, fin de funcion
	TD3_MEMCOPY_LEN equ ($ - _TD3_memcopy)









; void hlt();
; Funcion de HALT!
_hlt:
	.LFB0:
		hlt 
		jmp .LFB0
	.LFE0:
	STOP_LEN equ ($ - _hlt)









; Completa con NOPs hasta ROM_POS_RESET
	CODE_END equ ($)
	times (ROM_POS_RESET - ROM_POS_START - CODE_LEN) nop









; void start();
; Funcion donde comienza a ejecutar el procesador
_start:					; Procesador comienza en posicion 0xFFFFFFF0
	.LFB0:				; A Partir de aca tengo 16 bytes de codigo para definir mi futuro!!!!
		cli
		cld

		test EAX, 0x0 	; Verificar que el uP no este en fallo
		jne _hlt		; Jump if Not Equal (salta si eax != 0)

		jmp _start16 	; Salto relativo al comienzo de la ROM (mediante una etiqueta). 
	.LFE0:
	START_LEN equ ($ - _start)










; Completa con NOPs hasta ROM_SIZE
	align 16			; Alinea (rellena con NOP) hasta completar 16 bits

	%if (($ - CODE_START) != ROM_SIZE)
		%error Tamaño de ROM incorrecto
	%endif