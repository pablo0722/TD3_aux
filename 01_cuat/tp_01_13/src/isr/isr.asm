;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .isr

GLOBAL _IrqDE
GLOBAL _IrqNMI
GLOBAL _IrqBP
GLOBAL _IrqOF
GLOBAL _IrqBR
GLOBAL _IrqUD
GLOBAL _IrqNM
GLOBAL _IrqDF
GLOBAL _IrqTS
GLOBAL _IrqNP
GLOBAL _IrqSS
GLOBAL _IrqGP
GLOBAL _IrqPF
GLOBAL _IrqMF
GLOBAL _IrqAC
GLOBAL _IrqMC
GLOBAL _IrqXF

GLOBAL _IrqTM0
GLOBAL _IrqKB

EXTERN _isr_de 	; Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
EXTERN _isr_nmi
EXTERN _isr_bp
EXTERN _isr_of
EXTERN _isr_br
EXTERN _isr_ud 	; Invalid Opcode Exception
EXTERN _isr_nm
EXTERN _isr_df 	; Doble Fault
EXTERN _isr_ts
EXTERN _isr_np
EXTERN _isr_ss
EXTERN _isr_gp 	; General Protection
EXTERN _isr_pf 	; Page Fault
EXTERN _isr_mf
EXTERN _isr_ac
EXTERN _isr_mc
EXTERN _isr_xf

EXTERN _isr_tm0	; TIMER0 	irq 0 del PIC 1
EXTERN _isr_kb 	; KeyBoard 	irq 1 del PIC 1

EXTERN tasks_vector
EXTERN tasks_vector_idx
EXTERN tasks_vector_idx_prev
EXTERN tasks_vector_len
EXTERN task_status_len

USE32










;********************************************************
;						INCLUDES						*
;********************************************************
%include "x86/x86.inc"
%include "sys_tables/tss.inc"










;************************************************************
;						IRQs	                        	*
;************************************************************
_IrqDE:					; Divide Error Exception ( division por 0, o cuando sizeof(DIV(xx)) > sizeof(destino) )
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 0
		call _isr_de
		popad

		iret
	.LFE:

_IrqNMI:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 2
		call _isr_nmi
		popad

		iret
	.LFE:

_IrqBP:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 3
		call _isr_bp
		popad

		iret
	.LFE:

_IrqOF:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 4
		call _isr_of
		popad

		iret
	.LFE:

_IrqBR:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 5
		call _isr_br
		popad

		iret
	.LFE:

_IrqUD:					; Invalid Opcode Exception
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 6
		call _isr_ud
		popad

		iret
	.LFE:

_IrqNM:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 7
		call _isr_nm
		popad

		iret
	.LFE:

_IrqDF:					; Doble Fault
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 8
		call _isr_df
		popad

		iret
	.LFE:

_IrqTS:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 10
		call _isr_ts
		popad

		iret
	.LFE:

_IrqNP:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 11
		call _isr_np
		popad

		iret
	.LFE:

_IrqSS:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 12
		call _isr_ss
		popad

		iret
	.LFE:

_IrqGP:					; General Protection
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 13
		call _isr_gp
		popad

		iret
	.LFE:

_IrqPF:					; Page Fault
	.LFB:
		MBP _ISR1

		pop ebx ; error_code
		pushad
		mov edx, 14
		mov eax, cr2
		CALL _isr_pf, eax, ebx
		popad

		iret
	.LFE:

_IrqMF:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 16
		call _isr_mf
		popad

		iret
	.LFE:

_IrqAC:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 17
		call _isr_ac
		popad

		iret
	.LFE:

_IrqMC:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 18
		call _isr_mc
		popad

		iret
	.LFE:

_IrqXF:
	.LFB:
		MBP _ISR1

		pushad
		mov edx, 19
		call _isr_xf
		popad

		iret
	.LFE:

_IrqTM0: 				; Timer 0
	.LFB:
		MBP _ISR2

		; Asi queda el stack:
		;
		; ??? (valor siguiente)
		; ret_dir (EIP)  	 		<-- Posicion del Stack Pointer
		; Code Sel (CS) ; obtiene RPL
		; EFLAGS
		; ESP3 (cuando hay cambio de privilegio)
		; SS3 (cuando hay cambio de privilegio)
		; ... (valor previo)
		; ... (valor previo)


		; CPL (current Privilege Level) esta en la parte escondida de CS
		; RPL (Request Privilege Level)
		; DPL (Descriptor Privilege Level)

 		; Comparo RPL con el CPL:
 		;     	Si RPL > CPL, lee SP3 y SS3.
 		; 		

 		; System Call:
 		;     - CallGate
 		;     - IntGate (int 0x80) <- Usar esta. Debe tener DPL = 3 (Usuario) en la IDT del int 0x80

		;pushad
		;
		;call _isr_tm0
		;
		;mov al,20h			; Comando de acknowledge de la interrupcion actual
		;out 20h,al  		; acknowledge la interrupcion del PIC 1
		;
		;popad





		; vvv CAMBIO DE CONTEXTO vvv
		
		; Salvo contexto
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.ebp], ebp
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.eax], eax
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.ebx], ebx
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.ecx], ecx
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.edx], edx
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.esi], esi
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.edi], edi

		mov    	eax, [esp+8]
		mov 	ebx, [esp+4]
		mov 	ecx, [esp]
		or 		eax, 0x00000202
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.eip], 		ecx
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.cs], 		bx
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.eflags], 	eax
		add esp, 12
		mov [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.esp], esp

		; Acknowledge de Timer
		mov al,20h			; Comando de acknowledge de la interrupcion actual
		out 20h,al  		; acknowledge la interrupcion del PIC 1



		; Actualizar delay
		mov ecx, [tasks_vector_len]
		cmp ecx, 0x00
		je  .update_delay
		dec ecx
	.update_delay:
		mov eax, task_status_len
		mul ecx
		add eax, task_status_t.busy
		add eax, tasks_vector
		mov edx, [eax]
		cmp edx, 0x00
		je  .LL0
		dec edx
		mov [eax], edx
	.LL0:
		loop .update_delay

	.init_switch:
		mov ebx, [tasks_vector_idx]
		inc ebx
		cmp ebx, [tasks_vector_len]
		jne .idx_cyclic
		mov ebx, 0x01

	.idx_cyclic:
		mov ecx, ebx

	.is_busy:
		mov eax, task_status_len
		mul ecx
		add eax, task_status_t.busy
		add eax, tasks_vector
		mov edx, [eax]
		cmp edx, 0x00
		je  .task_switch

	.inc:
		inc ecx
		cmp ecx, [tasks_vector_len]
		jne .check_repeat
		mov ecx, 0x01
	.check_repeat:
		cmp ecx, ebx
		jne .is_busy
		mov ecx, 0x00

	.task_switch:
		mov [tasks_vector_idx], ecx
		mov eax, task_status_len
		mul ecx
		add eax, task_status_t.cr3
		add eax, tasks_vector
		mov edx, [eax]
		mov cr3, edx

		; Recupero contexto
		mov esp, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.esp]
		mov eax, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.eflags]
		mov bx , [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.cs] 
		mov ecx, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.eip]

		push eax
		push ebx
		push ecx

		mov edi, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.edi]
		mov esi, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.esi]
		mov edx, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.edx]
		mov ecx, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.ecx]
		mov ebx, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.ebx]
		mov eax, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.eax]
		mov ebp, [__STACK_TAREA_NIVEL0_LIN + tss_custom_t.ebp]

		; ^^^ CAMBIO DE CONTEXTO ^^^





		iret
	.LFE:

_IrqKB:					; KeyBoard IRQ 1 del PIC 1
	.LFB:
		MBP _ISR2

		pushad

		xor eax, eax
		in al, PORT_A_8042

		mov ebx, eax

		CALL _isr_kb, ebx

		mov al,20h			; Comando de acknowledge de la interrupcion actual
		out 20h,al  		; acknowledge la interrupcion del PIC 1

		popad

		iret
	.LFE: