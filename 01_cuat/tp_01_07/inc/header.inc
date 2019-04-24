;********************************************************
;						INCLUDES						*
;********************************************************
%include "a20g.inc"
%include "processor-flags.inc"
%include "macros.inc"










;********************************************************
;						INCLUDES						*
;********************************************************
EXTERN _hlt

EXTERN __STACK_SIZE_32
EXTERN __STACK_END_32

EXTERN __SHDW_HANDLERS_LEN
EXTERN __SHDW_HANDLERS_ORG
EXTERN __SHDW_HANDLERS_DST

EXTERN __SHDW_KERNEL32_LEN
EXTERN __SHDW_KERNEL32_ORG
EXTERN __SHDW_KERNEL32_DST

EXTERN __SHDW_SYS_TABLES_LEN
EXTERN __SHDW_SYS_TABLES_ORG
EXTERN __SHDW_SYS_TABLES_DST

EXTERN __DIGIT_TABLE_START
EXTERN __DIGIT_TABLE_SIZE_B
EXTERN __DIGIT_TABLE_END

EXTERN _gdtr
EXTERN _idtr
EXTERN CODE_SEL0
EXTERN DATA_SEL0

EXTERN __IRQ_DE_L
EXTERN __IRQ_DE_H