;********************************************************
;						MACROS							*
;********************************************************
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
%macro IF_EQ 3-* ; if(%1 == %2) {%3}
	cmp %1, %2
	jnz %%end

	%rep %0 - 2
		%3
		%rotate 1 
	%endrep 
%%end:
%endmacro