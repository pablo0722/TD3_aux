;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .funciones

GLOBAL _hlt
GLOBAL _TD3_memcopy

USE16










;********************************************************
;						FUNCIONES						*
;********************************************************
; void hlt();
; Funcion de HALT!
_hlt:
	.LFB0:
		hlt 
		jmp .LFB0
	.LFE0:
	STOP_LEN equ ($ - _hlt)










; void *TD3_memcopy(void *destino, const void *origen, unsigned int num_bytes);
; Copia <num_bytes> bytes de la posicion <origen> en <destino>
_TD3_memcopy:
	.LFB0:					; // Local Function Begin: etiqueta local, comienzo de funcion
		push EBP			; // Guarda el Base Pointer (porque despues se va a pisar)
		mov	 EBP, ESP		; BP = SP;

	;	mov 		SP, -N 	; Reservo variables locales

		push EDI
		push ESI
		push ECX
	.LFB1:					; Comienza codigo
							; [EBP]: antiguo EBP, [EBP+4]: antiguo IP (ocupa 2 bytes en call near), 
							; [EBP+6]: primer argumento de la izquierda de la funcion, [EBP+10]: segundo argumento, etc
							; [EBP-4]: primer variable local, [EBP-8]: segunda variable local, etc.
		mov	EAX,	[BP+6]	; valor de return = destino;
		mov	EDI,	EAX		; ES:DI = destino
		mov	ESI,	[BP+10]	; DS:SI = origen
		mov	ECX,	[BP+14]	; ECX = num_bytes;
	.LLB0:					; Local Loop Begin
		mov 	bl, [CS:ESI]
		mov  	[ES:EDI], bl
		inc 	ESI
		inc  	EDI
		loopnz	.LLB0
	.LLE0:					; Local Loop End
	.LFE1:					; Termina codigo
		pop	ECX
		pop	ESI
		pop	EDI

	;	mov			SP, BP	; SP = BP; Elimina las variables locales (en este ejemplo no hay)
		
		pop	EBP				; // Restaura el Base Pointer
		ret 				; Return;
	.LFE0:					; // Local Function End: etiqueta local, fin de funcion
	TD3_MEMCOPY_LEN equ ($ - _TD3_memcopy)
