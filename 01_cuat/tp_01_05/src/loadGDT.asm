;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .loadGDT

EXTERN __STACK_SEG_16
EXTERN __STACK_OFFSET_16
EXTERN __SHDW_HALT_LEN
EXTERN __SHDW_HALT_ORG
EXTERN __SHDW_HALT_DST
EXTERN __SHDW_RUTN_LEN
EXTERN __SHDW_RUTN_ORG
EXTERN __SHDW_RUTN_DST
EXTERN __SHDW_RUTN_JMP
EXTERN _init32
EXTERN rom_gdtr
EXTERN CODE_SEL0
EXTERN DATA_SEL0

GLOBAL _loadGDT

USE16










;********************************************************
;						INCLUDES						*
;********************************************************
%include "macros.inc"
%include "processor-flags.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void loadGDT();
_loadGDT:
	.LFB0:
		XCHG bx, bx
		o32 lgdt  [cs:rom_gdtr]

		;->Establecer el up en MP<-
		smsw  ax
		or	 ax, X86_CR0_PE
		lmsw  ax

		; No es necesario limpiar el prefetch queue ya que el siguiente far jump mismo lo hace
		;jmp   .flush_prefetch_queue
		;.flush_prefetch_queue:

		;	PASA A MODO PROTEGIDO
		o32 jmp dword CODE_SEL0:_init32
	.LFE0: