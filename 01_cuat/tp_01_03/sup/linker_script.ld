/* VMA (Virtual Memory Address): Direccion donde se va a ejecutar (usualmente en la RAM) */
/* LMA (Load Memory Address): Direccion donde se carga en la ROM */










/****************************************************
*                   DEFINICIONES INTERNAS           *
****************************************************/
size_all = 0x100000000;
reset_vector = 0xFFFFFFF0;










/****************************************************
*                   MEMORIA                         *
****************************************************/
MEMORY
{
    /*label (attr) :                /* label: etiqueta para representar sector de memoria (attr): attributos. permite: r(read), w(write), x(ejecucion), a(allocable, i(inicializada), !(invierte los atributos) */
    /*      ORIGIN = 0x00000000,    /* comienzo de la memoria */
    /*      LENGTH = 0xFFFF0000     /* tamaño de la memoria */
    /**/




    ram (!x) :  
        ORIGIN = 0x00000000,
        LENGTH = 0xFFFEFFFF





    rom (rx) :  
        ORIGIN = 0xFFFF0000,
        LENGTH = 0x0000FFF0





    rom_rset (rx) :  
        ORIGIN = 0xFFFFFFF0,
        LENGTH = 0x00000010
}










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
    /*} > rom16 AT > ram16                    /* Asigna la memoria a la cual pertenece para que pueda controlar que no desborde. La primera define la VMA, la segunda define la LMA */
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





    .main16 :
    {
        *(.main16);
    } > rom           





    .funciones :
    {
        *(.funciones);
    } > ram AT > rom





    fill_rom16 :
    {
        FILL(0x90);
        . = ORIGIN(rom_rset) - 1;
        BYTE(0x90);
    } > rom





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
__STACK_SEG_16 = 0x7000;
__STACK_OFFSET_16 = 0xFFFF;

__SHDW1_LEN = SIZEOF(.funciones);
__SHDW1_ORG = LOADADDR(.funciones);
__SHDW1_DST = ADDR(.funciones);
__SHDW1_JMP = ADDR(.funciones);
