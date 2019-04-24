;********************************************************
;						FUNCIONES						*
;********************************************************
; funciones en assembler
EXTERN _TD3_memcopy
EXTERN _keyboardPoll

EXTERN _idt_init
EXTERN idtr

; funciones en C
EXTERN _cr3_init 	; void _cr3_init()
EXTERN _pde_init 	; void _pde_init()
EXTERN _pte_init 	; void _pte_init()

; [TEST] Generadores de interrupciones
EXTERN _IntGenDE
EXTERN _IntGenNMI
EXTERN _IntGenBP
EXTERN _IntGenOF
EXTERN _IntGenBR
EXTERN _IntGenUD
EXTERN _IntGenNM
EXTERN _IntGenDF
EXTERN _IntGenTS
EXTERN _IntGenNP
EXTERN _IntGenSS
EXTERN _IntGenGP
EXTERN _IntGenPF
EXTERN _IntGenMF
EXTERN _IntGenAC
EXTERN _IntGenMC
EXTERN _IntGenXF










;************************************************************
;						INICIALIZACION                     	*
;************************************************************
EXTERN _PIC_Config










;************************************************************
;						IRQs	                        	*
;************************************************************
EXTERN _IrqDE
EXTERN _IrqNMI
EXTERN _IrqBP
EXTERN _IrqOF
EXTERN _IrqBR
EXTERN _IrqUD
EXTERN _IrqNM
EXTERN _IrqDF
EXTERN _IrqTS
EXTERN _IrqNP
EXTERN _IrqSS
EXTERN _IrqGP
EXTERN _IrqPF
EXTERN _IrqMF
EXTERN _IrqAC
EXTERN _IrqMC
EXTERN _IrqXF
