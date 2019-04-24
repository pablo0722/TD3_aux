;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .funciones

GLOBAL _TD3_memcopy
GLOBAL _load_cr3
GLOBAL _paging_enable
GLOBAL _load_gdt
GLOBAL _load_idt
GLOBAL _hlt

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

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "x86/x86.inc"










;************************************************************
;						INICIALIZACION	                   	*
;************************************************************










;************************************************************
;						FUNCIONES                        	*
;************************************************************
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










; void _load_cr3(dword cr3);
_load_cr3:
	.LFB:
		mov eax, [esp+4]
		mov cr3, eax
		ret
	.LFE:










; void _paging_enable();
_paging_enable:
	.LFB:
		mov eax, cr0
		or	eax, 0x80000000
		mov cr0, eax
		ret
	.LFE:










; void _load_gdt(Gdtr gdtr);
_load_gdt:
	.LFB:
		o32 lgdt  [cs:esp+4]

		ret
	.LFE:










; void _load_idt(Idtr idtr);
_load_idt:
	.LFB:
		cli
		lidt [esp+4]
		sti
		
		ret
	.LFE:










_hlt:
	.LFB:
		hlt
		ret
	.LFE:










;************************************************************
;						IRQs	                        	*
;************************************************************
_IntGenDE:
	.LFB:
		mov bx, 0x00
		mov ax, 0x00
		div bx
		ret
	.LFE:

_IntGenNMI:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenBP:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenOF:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenBR:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenUD:
	.LFB:
		LOCK mov ax, bx		; Hacer un MOV con el prefijo LOCK genera un #UD
		ret
	.LFE:

_IntGenNM:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenDF:
	.LFB:
		; Para hacer Double Fault, se debe generar una interrupcion en la vectorización de la primer interrupcion
		MBP 7
		mov esp, 0x0
		mov bx, 0x00
		mov ax, 0x00
		div bx
		ret
	.LFE:

_IntGenTS:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenNP:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenSS:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenGP:
	.LFB:
		MBP 7
		mov eax, 0xffffffff
		mov ss, eax
		ret
	.LFE:

_IntGenPF:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenMF:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenAC:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenMC:
	.LFB:
		hlt
		ret
	.LFE:

_IntGenXF:
	.LFB:
		hlt
		ret
	.LFE: