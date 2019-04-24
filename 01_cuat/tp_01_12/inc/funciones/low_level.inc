;********************************************************
;						FUNCIONES						*
;********************************************************
EXTERN _TD3_memcopy
EXTERN _get_key

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

EXTERN _ring_buffer_init
EXTERN _ring_buffer_push
EXTERN _ring_buffer_pop
EXTERN _ring_buffer_is_full
EXTERN _ring_buffer_is_empty










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

EXTERN _IrqKB
