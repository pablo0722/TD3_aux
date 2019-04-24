;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .reset

GLOBAL _reset

EXTERN _init16

USE16










;********************************************************
;						INCLUDES						*
;********************************************************
%include "header.inc"
%include "funciones.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void start();
; Funcion donde comienza a ejecutar el procesador
_reset:					; Procesador comienza en posicion 0xFFFFFFF0
	.LFB0:				; A Partir de aca tengo 16 bytes de codigo para definir mi futuro!!!!
		cli
		cld

		jmp 0xf000:_init16 	; Salto relativo al comienzo del programa 
	.LFE0: