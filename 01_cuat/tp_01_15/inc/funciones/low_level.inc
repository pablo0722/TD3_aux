;********************************************************
;						FUNCIONES						*
;********************************************************
EXTERN _TD3_memcopy
EXTERN _get_key

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
