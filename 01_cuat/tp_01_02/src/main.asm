%if 0 ; COMENTARIO
	TP1, Ej 2.
	I NICIALIZACIÓN BÁSICA UTILIZANDO SOLO ENSAMBLADOR CON ACCESO A  1MB 
	Escribir un programa que se ejecute en una ROM de 64kB y permita copiarse a si mismo en 
	cualquier zona de memoria. A tal fin se deberá implementar la función  
	void *td3_memcopy(void *destino, const void *origen, unsigned int num_bytes); 
	 
	Para validar el correcto funcionamiento del programa, el mismo deberá copiarse en las 
	direcciones indicadas a continuación y mediante Bochs verificar la memoria se halla escrito 
	correctamente. 
	i.  0x00000 
	ii. 0xF0000


	El A20 esta deshabilitado para este ejericio
	Por ahora podemos usar la RAM sin habilitarla ("bug" del bochs)
%endif









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

		; SS = 0xF000 - SP = 0xFFFF (apunta al final del 1er Mega de memoria RAM)
		mov AX, 0x7000 		; No se puede copiar directamente a SS. Hay que hacerlo mediante un registro
		mov SS, AX
		mov SP, 0xFFFF
		; Antes de inicializar el SP, hay que inicializar el controlador de la RAM. (no se hace en este ejemplo)

		; LLamo a la funcion void *TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes)
		push dword	(CODE_END - _main) 	; unsigned int num_bytes
		push dword 	_main 				; const void *origen
		push dword	0x00000000 			; void *destino
		call _TD3_memcopy				; void *TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes)
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
		push 	EBP					; // Guarda el Base Pointer (porque despues se va a pisar)
		mov		EBP, ESP			; EBP = ESP;

	;	mov 		SP, -N 			; Reservo variables locales

		push	EDI
		push	ESI
		push	ECX
	.LFB1:							; Comienza codigo
		mov AX, 0xF000
		mov DS, AX 					; DS = 0;
		xor AX, AX 					; DS = 0;
		mov ES, AX 					; ES = 0;
									; [EBP]: antiguo EBP, [EBP+4]: antiguo IP (2 bytes en call near), 
									; [EBP+6]: primer argumento de la izquierda de la funcion, [EBP+10]: segundo argumento, etc
									; [EBP-4]: primer variable local, [EBP-8]: segunda variable local, etc.
		mov		EAX,	[EBP+6]		; valor de return = destino;
		mov		EDI,	EAX			; ES:DI = destino
		mov		ESI,	[EBP+10]	; DS:SI = origen
		mov		ECX,	[EBP+14]	; ECX = num_bytes;
		a32 repnz 	movsb 				; repnz: repite instruccion <CX> veces
									; movsb: [ES:DI++] = [DS:SI++]; // (incrementa despues)
	.LFE1:							; Termina codigo
		pop		ECX
		pop		ESI
		pop		EDI

	;	mov			SP, BP		; SP = BP; Elimina las variables locales (en este ejemplo no hay)
		
		pop		EBP				; // Restaura el Base Pointer
		ret 					; Return;
	.LFE0:						; // Local Function End: etiqueta local, fin de funcion
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

		jmp 0xf000:_firmware_shadow 		; Salto relativo al comienzo de la ROM (mediante una etiqueta). 
	.LFE0:
	START_LEN equ ($ - _start)





align 16			; Alinea (rellena con NOP) hasta completar 16 bits

%if (($ - CODE_START) != ROM_SIZE)
	%error Tamaño de ROM incorrecto $(ROM_SIZE) %[$]%[CODE_START]
%endif