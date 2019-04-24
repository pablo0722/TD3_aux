;********************************************************
;						INCLUDES						*
;********************************************************
%include "x86/macros.inc"
%include "x86/processor-flags.inc"
%include "x86/a20g.inc"
%include "sys_tables/gdt.inc"










;********************************************************
;						VARIABLES						*
;********************************************************
EXTERN _init32










;********************************************************
;						DEFINES LINKER					*
;********************************************************
EXTERN __STACK_SEG_16
EXTERN __STACK_END_16

EXTERN __SHDW_GDT16_SEG
EXTERN __SHDW_GDT16_ORG
EXTERN __SHDW_GDT16_DST
EXTERN __SHDW_GDT16_LEN

EXTERN __SHDW_INIT32_SEG
EXTERN __SHDW_INIT32_ORG
EXTERN __SHDW_INIT32_DST
EXTERN __SHDW_INIT32_LEN

EXTERN __SHDW_SYS_TABLES16_SEG
EXTERN __SHDW_SYS_TABLES16_ORG
EXTERN __SHDW_SYS_TABLES16_DST
EXTERN __SHDW_SYS_TABLES16_LEN