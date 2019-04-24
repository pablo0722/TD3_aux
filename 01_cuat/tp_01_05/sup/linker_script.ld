/* Linker script */
/* Version: 2.0 */

/* VMA (Virtual Memory Address): Direccion donde se va a ejecutar (usualmente en la RAM) */
/* LMA (Load Memory Address): Direccion donde se carga en la ROM */










/****************************************************
*                   MEMORIA                         *
****************************************************/
MEMORY
{
    /*label (attr) :                /* label: etiqueta para representar sector de memoria (attr): 
    	attributos. permite: r(read), w(write), x(ejecucion), a(allocable, i(inicializada), 
    	!(invierte los atributos) */
    /*      ORIGIN = 0x00000000,    /* comienzo de la memoria */
    /*      LENGTH = 0xFFFF0000     /* tamaño de la memoria */
    /**/
    mem_all (rx) :    ORIGIN = 0x00000000, LENGTH = 0x100000000
    ram (wrx) :       ORIGIN = 0x00000000, LENGTH = 0xFFFF0000
    ram_rutn (rx) :   ORIGIN = 0x00000000, LENGTH = 0x00300000
    ram_code (rx) :   ORIGIN = 0x00300000, LENGTH = 0x00400000
    ram_stck_16 (!x) :ORIGIN = 0x00070000, LENGTH = 0x0000FFFF
    ram_stck_32 (!x) :ORIGIN = 0x00070000, LENGTH = 0x1FF8B000
    rom_code (rx) :   ORIGIN = 0xFFFF0000, LENGTH = 0x0000FFF0
    rom_rset (rx) :   ORIGIN = 0xFFFFFFF0, LENGTH = 0x00000010
}










/****************************************************
*                   DEFINICIONES INTERNAS           *
****************************************************/










/****************************************************
*                   SECCIONES                       *
****************************************************/
SECTIONS
{
    /*. = 0xFFFF00000;                    /* Setea el location counter (registro interno del linker) en 0xFFFFFFF0 */
    /**/





    /*.main16 ALIGN(16) :                 /* .main16: Output section. no tiene nada q ver con las secciones del asm. Seguidamente se setea una VMA, si no se define explicitamente, toma el location counter alineado a la alineacion mas estricta del output section. */
    /* AT ( 0xfffffd00 )                    /* Permite definir la LMA. */
    /*{                                       /* Se eligen las secciones de archivos especificos */
    /*    *(.main16);                         /* *: todos los archivos. (.main16): input seccions. */
    /*} > ram16 AT > rom16                    /* Asigna la memoria a la cual pertenece para que pueda controlar que no desborde. La primera define la VMA, la segunda define la LMA */
    /**/





    /*OVERLAY 0x00000000 :                /* Define secciones todas con la misma VMA */
    /*AT (.)                                  /* Permite definir una LMA. La primer seccion comenzara en la LMA definida, las siguientes secciones continuaran desde el final de la seccion anterior */
    /*{
    /*    .text0
    /*    {
    /*        *(.text)
    /*    }
    /*    
    /*    .text1 
    /*    { 
    /*        *(.text) 
    /*    }
    /*}
    /**/





    . = ORIGIN(rom_code);





    .init16 :
    {
        *(.init16);
    } > rom_code AT > rom_code












    .hlt :
    {
        *(.hlt);
    } > ram_rutn AT > rom_code

ram_rutn: direccion virtual (VMA: Virtual Memry Address)
rom_code: direccion fisica (LMA: Load Memory Address)










    .rutinas :
    {
        *(.rutinas);
        *(.init32);
        *(.loadGDT);
        *(.sys_tables);
    } > ram_rutn AT > rom_code





    .code :
    {
        *(.main32);
    } > ram_code AT > rom_code






    fill_rom16 :
    {
        FILL(0x90);
        . = ORIGIN(rom_rset) - 1;
        BYTE(0x90);
    } > rom_code





    . = ORIGIN(rom_rset);





    .reset :
    {
        *(.reset);
    } > rom_rset





    end_rom16 :
    {
        FILL(0x90);
        . = ORIGIN(rom_rset) + LENGTH(rom_rset) - 1;
        BYTE(0x90);
    } > rom_rset
}










/****************************************************
*                   DEFINICIONES EXTERNAS           *
****************************************************/
__STACK_SEG_16 = ORIGIN(ram_stck_16) / 16;
__STACK_OFFSET_16 = LENGTH(ram_stck_16);

__STACK_SIZE_32_DW = LENGTH(ram_stck_32) / 4;
__STACK_SIZE_32_W = LENGTH(ram_stck_32) / 2;
__STACK_SIZE_32_B = LENGTH(ram_stck_32);
__STACK_END_32 = ORIGIN(ram_stck_32) + LENGTH(ram_stck_32);

__SHDW_HALT_LEN = SIZEOF(.hlt);
__SHDW_HALT_ORG = LOADADDR(.hlt);
__SHDW_HALT_DST = ADDR(.hlt);

__SHDW_RUTN_LEN = SIZEOF(.rutinas);
__SHDW_RUTN_ORG = LOADADDR(.rutinas);
__SHDW_RUTN_DST = ADDR(.rutinas);
__SHDW_RUTN_JMP = ADDR(.rutinas);

__SHDW_CODE_LEN = SIZEOF(.code);
__SHDW_CODE_ORG = LOADADDR(.code);
__SHDW_CODE_DST = ADDR(.code);
__SHDW_CODE_JMP = ADDR(.code);
