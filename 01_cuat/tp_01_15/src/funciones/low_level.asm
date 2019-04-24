;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .kfunciones

EXTERN TSS_SEL

GLOBAL _TD3_memcopy
GLOBAL _get_cr3
GLOBAL _set_cr3
GLOBAL _paging_enable
GLOBAL _load_gdt
GLOBAL _load_idt
GLOBAL _load_tr

; *** SYSTEM CALLS *** ;
GLOBAL _hlt
GLOBAL _task_hlt
GLOBAL _read
GLOBAL _task_delay
GLOBAL _screen_reset
GLOBAL _screen_set_color
GLOBAL _screen_get_color
GLOBAL _screen_write_char
GLOBAL _screen_write_char_color
GLOBAL _screen_write_str
GLOBAL _screen_write_str_color
GLOBAL _screen_write_str_nl
GLOBAL _screen_write_str_nl_color
GLOBAL _screen_write_byte
GLOBAL _screen_write_word
GLOBAL _screen_write_dword
GLOBAL _screen_write_dword_color
GLOBAL _screen_write_byte_hex
GLOBAL _screen_write_word_hex
GLOBAL _screen_write_dword_hex
GLOBAL _screen_write_dword_hex_color

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










; void dword _get_cr3();
_get_cr3:
	.LFB:
		mov eax, cr3
		ret
	.LFE:










; void _set_cr3(dword cr3);
_set_cr3:
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










; void _load_tr();
_load_tr:
	.LFB:
		push eax
		mov eax, TSS_SEL
		ltr ax
		pop eax

		ret
	.LFE:










_hlt:
	.LFB:
		hlt
		ret
	.LFE:









; ***** SYSTEM_CALLS ***** ;
SECTION .funciones





ENUM \
	SC_TASK_HALT, \
	SC_TASK_DELAY, \
	SC_READ, \
	SC_SCREEN_RESET, \
	SC_SCREEN_SET_COLOR, \
	SC_SCREEN_GET_COLOR, \
	SC_SCREEN_WRITE_CHAR, \
	SC_SCREEN_WRITE_CHAR_COLOR, \
	SC_SCREEN_WRITE_STR, \
	SC_SCREEN_WRITE_STR_COLOR, \
	SC_SCREEN_WRITE_NL, \
	SC_SCREEN_WRITE_NL_COLOR, \
	SC_SCREEN_WRITE_BYTE, \
	SC_SCREEN_WRITE_WORD, \
	SC_SCREEN_WRITE_DWORD, \
	SC_SCREEN_WRITE_DWORD_COLOR, \
	SC_SCREEN_WRITE_BYTE_HEX, \
	SC_SCREEN_WRITE_WORD_HEX, \
	SC_SCREEN_WRITE_DWORD_HEX, \
	SC_SCREEN_WRITE_DWORD_HEX_COLOR










_task_hlt:
	.LFB:
		push eax
		mov eax, SC_TASK_HALT
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax
		pop eax

		ret
	.LFE:










_task_delay:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_TASK_DELAY
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_read:
	.LFB:
		PUSH ebx, ecx

		mov eax, SC_READ
		mov ebx, [esp+12]
		mov ecx, [esp+16]
		
		int 0x80

		push eax
		mov eax, ss
		mov ds,  eax
		mov es,  eax
		pop eax

		POP ebx, ecx

		ret
	.LFE:










_screen_reset:
	.LFB:
		push eax
		mov eax, SC_SCREEN_RESET
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax
		pop eax

		ret
	.LFE:










_screen_set_color:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_SET_COLOR
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_get_color:
	.LFB:
		mov eax, SC_SCREEN_GET_COLOR
		
		int 0x80

		push eax
		mov eax, ss
		mov ds,  eax
		mov es,  eax
		pop eax

		ret
	.LFE:










_screen_write_char:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_CHAR
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_char_color:
	.LFB:
		PUSH eax, ebx, ecx

		mov eax, SC_SCREEN_WRITE_CHAR_COLOR
		mov ebx, [esp+12]
		mov ecx, [esp+16]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx, ecx

		ret
	.LFE:










_screen_write_str:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_STR
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_str_color:
	.LFB:
		PUSH eax, ebx, ecx

		mov eax, SC_SCREEN_WRITE_STR_COLOR
		mov ebx, [esp+12]
		mov ecx, [esp+16]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx, ecx

		ret
	.LFE:










_screen_write_str_nl:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_NL
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_str_nl_color:
	.LFB:
		PUSH eax, ebx, ecx

		mov eax, SC_SCREEN_WRITE_NL_COLOR
		mov ebx, [esp+12]
		mov ecx, [esp+16]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx, ecx

		ret
	.LFE:










_screen_write_byte:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_BYTE
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_word:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_WORD
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_dword:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_DWORD
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_dword_color:
	.LFB:
		PUSH eax, ebx, ecx

		mov eax, SC_SCREEN_WRITE_DWORD_COLOR
		mov ebx, [esp+12]
		mov ecx, [esp+16]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx, ecx

		ret
	.LFE:










_screen_write_byte_hex:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_BYTE_HEX
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_word_hex:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_WORD_HEX
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_dword_hex:
	.LFB:
		PUSH eax, ebx

		mov eax, SC_SCREEN_WRITE_DWORD_HEX
		mov ebx, [esp+12]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx

		ret
	.LFE:










_screen_write_dword_hex_color:
	.LFB:
		PUSH eax, ebx, ecx

		mov eax, SC_SCREEN_WRITE_DWORD_HEX_COLOR
		mov ebx, [esp+12]
		mov ecx, [esp+16]
		
		int 0x80

		mov eax, ss
		mov ds,  eax
		mov es,  eax

		POP eax, ebx, ecx

		ret
	.LFE: