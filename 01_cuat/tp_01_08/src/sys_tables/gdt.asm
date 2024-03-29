;********************************************************
;						DIRECTIVAS					 	*
;********************************************************
section .sys_tables; progbits ; @progbits: seccion contiene datos de programa

GLOBAL CODE_SEL0
GLOBAL DATA_SEL0
GLOBAL gdtr
GLOBAL gdtr16










;**********************************************************
;						ESTRUCTURAS						                        *
;**********************************************************
struc   gdtd_t      ; Definicion de la estructura denominada gdtd_t, la cual contiene los siguientes campos
       .limite:      resw 1   ; Limite del segmento bits 00-15.
       .base00_15:   resw 1   ; Direccion base del segmento bits 00-15.
       .base16_23:   resb 1   ; Direccion base del segmento bits 16-23.
       .attr:        resb 1   ; Atributos. Pr | Privl_1 | Privl_2 | 1 | Ex | DC | RW | Ac
       .lim_attr:    resb 1   ; Limite del segmento 16-19 y atributos. Gr | Sz | L | 0 | Limit 16:19
       .base24_31:   resb 1   ; Direccion base del segmento bits 24-31. 
endstruc

; Pr: Present bit. This must be 1 for all valid selectors.
; Privl: Privilege, 2 bits. Contains the ring level, 0 = highest (kernel), 3 = lowest (user applications).
; Ex: Executable bit. If 1 code in this segment can be executed, ie. a code selector. If 0 it is a data selector.
; DC: Direction bit/Conforming bit.
;     Direction bit for data selectors: Tells the direction.
;        0 the segment grows up.
;        1 the segment grows down, ie. the offset has to be greater than the limit.
;     Conforming bit for code selectors:
;        If 1 code in this segment can be executed from an equal or lower privilege level. For example, code in ring 3 can far-jump to conforming code in a ring 2 segment. The privl-bits represent the highest privilege level that is allowed to execute the segment. For example, code in ring 0 cannot far-jump to a conforming code segment with privl==0x2, while code in ring 2 and 3 can. Note that the privilege level remains the same, ie. a far-jump form ring 3 to a privl==2-segment remains in ring 3 after the jump.
;        If 0 code in this segment can only be executed from the ring set in Privl.
; RW: Readable bit/Writable bit.
;     Readable bit for code selectors: Whether read access for this segment is allowed. Write access is never allowed for code segments.
;     Writable bit for data selectors: Whether write access for this segment is allowed. Read access is always allowed for data segments.
; Ac: Accessed bit. Just set to 0. The CPU sets this to 1 when the segment is accessed.
;
; Gr: Granularity bit. If 0 the limit is in 1 B blocks (byte granularity), if 1 the limit is in 4 KiB blocks (page granularity).
; Sz: Size bit. If 0 the selector defines 16 bit protected mode. If 1 it defines 32 bit protected mode. You can have both 16 bit and 32 bit selectors at once. Sz has to be 0 when the 'L' bit is 1, as the combination Sz = 1, L = 1 is reserved for future use (and will throw an exception if you try to use it)
; L: indicate x86-64 descriptor (64-bits)

align 8
gdt:
NULL_SEL0   equ     $-gdt
Gdt_desc_nulo: istruc gdtd_t      ; Instancia Gdt_desc_nulo de la estructura gdtd_t
               at gdtd_t.limite,    dw 0x0000   ; Limite_00_15
               at gdtd_t.base00_15, dw 0x0000   ; Base_00_15
               at gdtd_t.base16_23, db 0x00     ; Base_16_23
               at gdtd_t.attr,      db 0x00     ; Atributos
               at gdtd_t.lim_attr,  db 0x00     ; Lim_attr
               at gdtd_t.base24_31, db 0x00     ; Base_24_31
               iend           ; Fin de instancia Gdt_desc_nulo

CODE_SEL0   equ     $-gdt
Gdt_desc_code_sel0: istruc gdtd_t         ; Instancia Gdt_desc_nulo de la estructura gdtd_t
               at gdtd_t.limite,    dw 0xFFFF      ; Limite_00_15
               at gdtd_t.base00_15, dw 0x0000      ; Base_00_15
               at gdtd_t.base16_23, db 0x0000      ; Base_16_23
               at gdtd_t.attr,      db 10011011b   ; Atributos. present | max prvl | 1 | code segment | Conforming strict | readable | Accesed
               at gdtd_t.lim_attr,  db 11001111b   ; Lim_attr
               at gdtd_t.base24_31, db 0x00        ; Base_24_31
               iend                       ; Fin de instancia Gdt_desc_nulo

DATA_SEL0   equ     $-gdt
Gdt_desc_data_sel0: istruc gdtd_t         ; Instancia Gdt_desc_nulo de la estructura gdtd_t
               at gdtd_t.limite,    dw 0xFFFF      ; Limite_00_15
               at gdtd_t.base00_15, dw 0x0000      ; Base_00_15
               at gdtd_t.base16_23, db 0x0000      ; Base_16_23
               at gdtd_t.attr,      db 10010011b   ; Atributos. present | max prvl  | 1 | data segment | Direction GrowUp | writable | Accesed
               at gdtd_t.lim_attr,  db 11001111b   ; Lim_attr
               at gdtd_t.base24_31, db 0x00        ; Base_24_31
               iend                       ; Fin de instancia Gdt_desc_nulo     
GDT_LENGTH  equ $-gdt

; The GDTR is 48 bits long. The lower 16 bits tell the size of the GDT (LIMIT = GDTR[00:15]), and the upper 32 bits tell the location of the GDT in memory (BASE = GDTR[16:47]).
; LIMIT is the size of the GDT, and BASE is the starting address. LIMIT is 1 less than the length of the table, so if LIMIT has the value 15, then the GDT is 16 bytes long.
gdtr:
   dw GDT_LENGTH-1
   dd gdt





section .gdtr16; progbits ; @progbits: seccion contiene datos de programa

; The GDTR is 48 bits long. The lower 16 bits tell the size of the GDT (LIMIT = GDTR[00:15]), and the upper 32 bits tell the location of the GDT in memory (BASE = GDTR[16:47]).
; LIMIT is the size of the GDT, and BASE is the starting address. LIMIT is 1 less than the length of the table, so if LIMIT has the value 15, then the GDT is 16 bytes long.
gdtr16:
   dw GDT_LENGTH-1
   dd 0x00000000