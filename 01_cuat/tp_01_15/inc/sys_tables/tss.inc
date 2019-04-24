;************************************************************
;						ESTRUCTURAS 						*
;************************************************************
EXTERN __STACK_TAREA_NIVEL0_LIN










;************************************************************
;						ESTRUCTURAS 						*
;************************************************************
struc   tss_intel_t      ; Definicion de la estructura denominada tss_intel_t, la cual contiene los siguientes campos
       .link:			resw 1
       .reserved1: 		resw 1
       .esp0:			resd 1
       .ss0:			resw 1
       .reserved2: 		resw 1
       .esp1:			resd 1
       .ss1:			resw 1
       .reserved3: 		resw 1
       .esp2:			resd 1
       .ss2:			resw 1
       .reserved4: 		resw 1
       .cr3:			resd 1
       .eip:			resd 1
       .eflags:			resd 1
       .eax:			resd 1
       .ecx:			resd 1
       .edx:			resd 1
       .ebx:			resd 1
       .esp:			resd 1
       .ebp:			resd 1
       .esi:			resd 1
       .edi:			resd 1
       .es:				resw 1
       .reserved5: 		resw 1
       .cs:				resw 1
       .reserved6: 		resw 1
       .ss:				resw 1
       .reserved7: 		resw 1
       .ds:				resw 1
       .reserved8: 		resw 1
       .fs:				resw 1
       .reserved9: 		resw 1
       .gs:				resw 1
       .reserved10:		resw 1
       .ldtr:			resw 1
       .reserved11: 	resw 1
       .reserved12: 	resw 1
       .iobp_offset: 	resw 1
endstruc





struc   tss_custom_t      ; Definicion de la estructura denominada tss_custom_t, la cual contiene los siguientes campos
       .eax:      	 resd 1
       .ebx:      	 resd 1
       .ecx:      	 resd 1
       .edx:      	 resd 1
       .ebp:      	 resd 1
       .esi:      	 resd 1
       .edi:      	 resd 1
       .eip:      	 resd 1
       .cs:      	 resw 1
       .esp3:      	 resd 1
       .ss3:      	 resw 1
       .esp0:      	 resd 1
       .ss0:      	 resw 1
       .eflags:      resd 1
       .empty: 		 resb 14
       .xsave_area:  resb 512
endstruc





struc   task_status_t      ; Definicion de la estructura denominada tss_custom_t, la cual contiene los siguientes campos
       .cr3:      	 resd 1
       .busy:      	 resd 1
endstruc