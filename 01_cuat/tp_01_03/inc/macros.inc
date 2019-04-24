;********************************************************
;						MACROS							*
;********************************************************
%macro MOV_SEG 3 ; segmento, valor, registro
    mov %3, %2
    mov %1, %3
%endmacro





%macro CLR_SEG 2 ; segmento, registro
    xor %2, %2
    mov %1, %2
%endmacro





; Greedy macro
%macro  multipush 1-* 
	%rep  %0 
		push    %1 
		%rotate 1 
	%endrep 
%endmacro





; Greedy macro
%macro  multipop 1-* 
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