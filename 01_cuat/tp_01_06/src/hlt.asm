;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .hlt

GLOBAL _hlt

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