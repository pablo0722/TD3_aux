;********************************************************
;						DIRECTIVAS					 	*
;********************************************************
section .sys_tables; progbits ; @progbits: seccion contiene datos de programa

EXTERN __PDT_TAREA0_PHY
EXTERN __PDT_TAREA1_PHY
EXTERN __PDT_TAREA2_PHY

EXTERN __STACK_TAREA_NIVEL0_LIN_END
EXTERN CODE_SEL0

EXTERN __TAREA_TEXT_LIN

GLOBAL tasks_vector
GLOBAL tasks_vector_idx
GLOBAL tasks_vector_idx_prev
GLOBAL tasks_vector_len
GLOBAL task_status_len










;********************************************************
;						INCLUDES						*
;********************************************************
%include "sys_tables/tss.inc"










;**********************************************************
;						ESTRUCTURAS						                        *
;**********************************************************
tasks_vector: istruc task_status_t 		; vector de la estructura task_status_t
	at task_status_t.cr3, 	dd __PDT_TAREA0_PHY
	at task_status_t.busy, 	dd 0x0000
	iend
task_status_len equ ($ - tasks_vector)

istruc task_status_t
	at task_status_t.cr3, 	dd __PDT_TAREA1_PHY
	at task_status_t.busy, 	dd 0x0000
	iend

istruc task_status_t
	at task_status_t.cr3, 	dd __PDT_TAREA2_PHY
	at task_status_t.busy, 	dd 0x0000
	iend





tasks_vector_idx:
	dd 0





tasks_vector_idx_prev:
	dd 0





tasks_vector_len:
	dd 3










section .bss.tss_tarea0

istruc tss_custom_t 		; vector de la estructura tss_custom_t
	at tss_custom_t.eax, 	dd 0
	at tss_custom_t.ebx, 	dd 0
	at tss_custom_t.ecx, 	dd 0
	at tss_custom_t.edx, 	dd 0
	at tss_custom_t.esp, 	dd __STACK_TAREA_NIVEL0_LIN_END
	at tss_custom_t.ebp, 	dd 0
	at tss_custom_t.esi, 	dd 0
	at tss_custom_t.edi, 	dd 0
	at tss_custom_t.eip, 	dd __TAREA_TEXT_LIN
	at tss_custom_t.cs, 	dw CODE_SEL0
	at tss_custom_t.eflags, dd 0x00000202
	iend





section .bss.tss_tarea1

istruc tss_custom_t 		; vector de la estructura tss_custom_t
	at tss_custom_t.eax, 	dd 0
	at tss_custom_t.ebx, 	dd 0
	at tss_custom_t.ecx, 	dd 0
	at tss_custom_t.edx, 	dd 0
	at tss_custom_t.esp, 	dd __STACK_TAREA_NIVEL0_LIN_END
	at tss_custom_t.ebp, 	dd 0
	at tss_custom_t.esi, 	dd 0
	at tss_custom_t.edi, 	dd 0
	at tss_custom_t.eip, 	dd __TAREA_TEXT_LIN
	at tss_custom_t.cs, 	dw CODE_SEL0
	at tss_custom_t.eflags, dd 0x00000202
	iend





section .bss.tss_tarea2

istruc tss_custom_t 		; vector de la estructura tss_custom_t
	at tss_custom_t.eax, 	dd 0
	at tss_custom_t.ebx, 	dd 0
	at tss_custom_t.ecx, 	dd 0
	at tss_custom_t.edx, 	dd 0
	at tss_custom_t.esp, 	dd __STACK_TAREA_NIVEL0_LIN_END
	at tss_custom_t.ebp, 	dd 0
	at tss_custom_t.esi, 	dd 0
	at tss_custom_t.edi, 	dd 0
	at tss_custom_t.eip, 	dd __TAREA_TEXT_LIN
	at tss_custom_t.cs, 	dw CODE_SEL0
	at tss_custom_t.eflags, dd 0x00000202
	iend