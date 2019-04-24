/****************************************************
*                   MEMORIA                         *
****************************************************/
MEMORY
{
    ram_handlers (rwxai) :  
        ORIGIN = 0x00000000,
        LENGTH = 0x00100000





    ram_sys_tables (rwai) :  
        ORIGIN = 0x00100000,
        LENGTH = 0x00200000





    ram_kernel32 (rwxai) :  
        ORIGIN = 0x00300000,
        LENGTH = 0x00010000





    ram_tabla_digitos (rwai) :  
        ORIGIN = 0x00310000,
        LENGTH = 0x000D0000




    ram_datos (rwai) :  
        ORIGIN = 0x003E0000,
        LENGTH = 0x00300000




    ram_stack (rwai) :  
        ORIGIN = 0x1FEFB000,
        LENGTH = 0x00100000





    rom_code (rx) :  
        ORIGIN = 0xFFFF0000,
        LENGTH = 0x0000FFF0





    rom_reset (rx) :  
        ORIGIN = 0xFFFFFFF0,
        LENGTH = 0x00000010
}










/****************************************************
*                   SECCIONES                       *
****************************************************/
SECTIONS
{
    .handlers :
    {
        *(.hlt);
        *(.isr);
        *(.init32);
        *(.funciones);
    } > ram_handlers AT > rom_code






    .sys_tables :
    {
        *(.sys_tables);
    } > ram_sys_tables AT > rom_code






    .kernel32 :
    {
        *(.kernel32);
    } > ram_kernel32 AT > rom_code






    .my_data :
    {
        *(.my_bss);
        *(.my_data);
    } > ram_datos AT > rom_code






    .init16 :
    {
        *(.init16);
    } > rom_code






    .fill_rom16 :
    {
        . = ORIGIN(rom_reset) - 1;
        BYTE(0x90);
    } > rom_code = 0x90





    .reset :
    {
        *(.reset);
    } > rom_reset





    .end_rom16 :
    {
        . = ORIGIN(rom_reset) + LENGTH(rom_reset) - 1;
        BYTE(0x90);
    } > rom_reset = 0x90
}










/****************************************************
*                   DEFINICIONES EXTERNAS           *
****************************************************/
__STACK_SIZE_32 = LENGTH(ram_stack) / 4;
__STACK_END_32 = ORIGIN(ram_stack) + LENGTH(ram_stack);

__SHDW_HANDLERS_LEN = SIZEOF(.handlers);
__SHDW_HANDLERS_ORG = LOADADDR(.handlers);
__SHDW_HANDLERS_DST = ADDR(.handlers);

__SHDW_KERNEL32_LEN = SIZEOF(.kernel32);
__SHDW_KERNEL32_ORG = LOADADDR(.kernel32);
__SHDW_KERNEL32_DST = ADDR(.kernel32);

__SHDW_SYS_TABLES_LEN = SIZEOF(.sys_tables);
__SHDW_SYS_TABLES_ORG = LOADADDR(.sys_tables);
__SHDW_SYS_TABLES_DST = ADDR(.sys_tables);

__DIGIT_TABLE_START = ORIGIN(ram_tabla_digitos);
__DIGIT_TABLE_SIZE_B = LENGTH(ram_tabla_digitos);
__DIGIT_TABLE_END = ORIGIN(ram_tabla_digitos) + LENGTH(ram_tabla_digitos);