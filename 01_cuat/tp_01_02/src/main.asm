; Compilo con: nasm rom.s -o rom.bin -l rom.lst
; *.s es el codigo en asembler
; *.bin es el binario. Se puede leer con: hexdump *.bin
; *.lst lista las instrucciones en el formato: fila, direccion, opcode y mnemonico. Se puede leer con: cat *.lst

; El A20 esta deshabilitado para este ejericio
; Por ahora podemos usar la RAM sin habilitarla ("bug" del bochs)









;********************************************************
;						DEFINES							*
;********************************************************
%define ROM_SIZE			(64*1024)
%define ROM_POS_START		0xf0000
%define ROM_POS_RESET		0xffff0


CODE_START equ ($)
%define CODE_LEN (CODE_END - CODE_START)










;********************************************************
;						DIRECTIVAS							*
;********************************************************
section .text
use16							; use16: Por default se usa 16-bits encoding para las instrucciones; mientras que para las instrucciones 32-bits encoding los operandos deben ser prefijados (la instruccion ocupa 2 bytes) permitiendo usar registros de 32-bits

global _start










;********************************************************
;						CODIGO							*
;********************************************************
; Funcion _firmware_shadow
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





; Funcion _main
_main:
		jmp _stop
	MAIN_LEN equ ($ - _main)





; Funcion _TD3_memcopy
_TD3_memcopy:						; void *TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes)
	.LFB0:							; // Local Function Begin: etiqueta local, comienzo de funcion
		push dword 	EBP				; // Guarda el Base Pointer (porque despues se va a pisar)
		mov	 dword	EBP, ESP		; BP = SP;

	;	mov 		SP, -N 			; Reservo variables locales

		push dword	EDI
		push dword	ESI
		push dword	ECX
	.LFB1:							; Comienza codigo
									; [EBP]: antiguo EBP, [EBP+4]: antiguo IP (2 bytes en call near), 
									; [EBP+6]: primer argumento de la izquierda de la funcion, [EBP+10]: segundo argumento, etc
									; [EBP-4]: primer variable local, [EBP-8]: segunda variable local, etc.
		mov dword	EAX,	[BP+6]	; valor de return = destino;
		mov dword	EDI,	EAX		; ES:DI = destino
		mov dword	ESI,	[BP+10]	; DS:SI = origen
		mov	dword	ECX,	[BP+14]	; ECX = num_bytes;
		repnz		movsb			; repnz: repite instruccion <CX> veces
									; movsb: [ES:DI++] = [DS:SI++]; // (incrementa despues)
	.LFE1:							; Termina codigo
		pop dword	ECX
		pop	dword	ESI
		pop	dword	EDI

	;	mov			SP, BP			; SP = BP; Elimina las variables locales (en este ejemplo no hay)
		
		pop dword	EBP				; // Restaura el Base Pointer
		ret 						; Return;
	.LFE0:							; // Local Function End: etiqueta local, fin de funcion
	TD3_MEMCOPY_LEN equ ($ - _TD3_memcopy)





; Funcion _stop
_stop:
	.LFB0:
		hlt 
		jmp .LFB0
	.LFE0:
	STOP_LEN equ ($ - _stop)





CODE_END equ ($)
times (ROM_POS_RESET - ROM_POS_START - CODE_LEN) nop





; Funcion _start
_start:					; comienza en posicion 0xFFFFFFF0
	.LFB0:				; A Partir de aca tengo 16 bytes de codigo para definir mi futuro!!!!
		cli 
		cld 

		; SS = 0xF000 - SP = 0xFFFF (apunta al final del 1er Mega de memoria RAM)
		mov AX, 0x1000 		; No se puede copiar directamente a SS. Hay que hacerlo mediante un registro
		mov SS, AX
		mov SP, 0xFFFF
		; Antes de inicializar el SP, hay que inicializar el controlador de la RAM

		jmp _firmware_shadow 		; Salto relativo al comienzo de la ROM (mediante una etiqueta). 
	.LFE0:
	START_LEN equ ($ - _start)





align 16			; Alinea (rellena con NOP) hasta completar 16 bits

%if (($ - CODE_START) != ROM_SIZE)
	%error Tamaño de ROM incorrecto
%endif