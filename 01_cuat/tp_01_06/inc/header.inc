;********************************************************
;						INCLUDES						*
;********************************************************
%include "A20G.inc"
%include "processor-flags.inc"
%include "macros.inc"










;********************************************************
;						INCLUDES						*
;********************************************************
EXTERN __STACK_SEG_16
EXTERN __STACK_OFFSET_16
EXTERN __STACK_SIZE_32_DW
EXTERN __STACK_END_32
EXTERN __SHDW_HALT_LEN
EXTERN __SHDW_HALT_ORG
EXTERN __SHDW_HALT_DST
EXTERN __SHDW_RUTN_LEN
EXTERN __SHDW_RUTN_ORG
EXTERN __SHDW_RUTN_DST
EXTERN __SHDW_RUTN_JMP
EXTERN __SHDW_CODE_LEN
EXTERN __SHDW_CODE_ORG
EXTERN __SHDW_CODE_DST
EXTERN __SHDW_CODE_JMP
EXTERN __DIGIT_TABLE_START
EXTERN __DIGIT_TABLE_SIZE_B
EXTERN __DIGIT_TABLE_END

EXTERN rom_gdtr
EXTERN CODE_SEL0
EXTERN DATA_SEL0