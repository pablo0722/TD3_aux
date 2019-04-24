;********************************************************
;						MACROS							*
;********************************************************
%define DEBUG 1










;********************************************************
;						MACROS							*
;********************************************************
; Greedy macro
%macro  ENUM 1-*
	%define %1 0
	%rep  %0 - 1
		%define %2 %1 + 1
		%rotate 1
	%endrep
%endmacro





%macro MOV_SEG 3 ; segmento, valor, registro
    mov %3, %2
    mov %1, %3
%endmacro





%macro CLR_SEG 2 ; segmento, registro
    xor %2, %2
    mov %1, %2
%endmacro





; Greedy macro
%macro  PUSH 1-* 
	%rep  %0 
		push    %1 
		%rotate 1 
	%endrep 
%endmacro





; Greedy macro
%macro  POP 1-* 
	%rep %0 
		%rotate -1 
		pop     %1 
	%endrep 
%endmacro





; Greedy overloaded macro
%macro CALL 1-* ; func, argList:vararg
	%rep %0 - 1 
		%rotate -1 
		push     %1 
	%endrep 
	%rotate -1 
	call %1
	add esp, 4*(%0 - 1)
%endmacro





; macro Magic BreakPoint
ENUM _ISR1, _ISR2, _INIT16, _INIT32, _GDT
%macro MBP 1 ; nivel de Magic Breakpoint
	%if %1 == _ISR1			; Nivel 1 para las IRQ
		;xchg bx, bx

	%elif %1 == _ISR2		; Nivel 2 para las IRQ de los PICs
		;xchg bx, bx

	%elif %1 == _INIT16
		;xchg bx, bx

	%elif %1 == _INIT32
		;xchg bx, bx

	%elif %1 == _GDT
		;xchg bx, bx
		
	%else
		xchg bx, bx
	%endif
%endmacro





; Greedy macro
%macro FOR 2-* ; registro, registro
	mov ecx, %1
%%LocalLoop:
	%rep %0 - 1
		%2
		%rotate 1 
	%endrep 
	loop %%LocalLoop
%endmacro





; Greedy macro
%macro IF 3-* ; if(%1 == %2) {%3}
	cmp %1, %2
	jnz %%end

	%rep %0 - 2
		%3
		%rotate 1 
	%endrep 
%%end:
%endmacro





; macro
%macro TABLE_INIT 1 ; %1: table_start
	mov dword [%1], %1
%%end:
%endmacro





; macro
%macro TABLE_PUSH 2 ; %1: table_start, %2: table_end
	cmp dword [%1], %2 	; No hay mas espacio en la tabla, lo descarto
	jz %%end

	add dword [%1], 4 	; En la primer posicion de memoria guardo el offset del ultimo valor guardado			
	mov ebx, [%1]
	mov [ebx], eax
%%end:
%endmacro





; macro
%macro TABLE_POP 1
	xor eax, eax
	cmp dword [%1], %1
	jz %%end 												; No hay teclas nuevas
			
	mov eax,  [%1]
	mov eax,  [eax]
	sub dword [%1], 4
%%end:
%endmacro