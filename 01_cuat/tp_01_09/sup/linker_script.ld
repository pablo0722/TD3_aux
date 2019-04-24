/****************************************************
*                   MEMORIA                         *
****************************************************/
MEMORY
{
    ram_init32 (rwai) :  
        ORIGIN = 0x00000020,
        LENGTH = 0x00010000





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





    ram_tarea_1 (rwai) :  
        ORIGIN = 0x00320000,
        LENGTH = 0x000D0000




    ram_datos (rwai) :  
        ORIGIN = 0x003E0000,
        LENGTH = 0x00100000




    ram_keyboard_buffer (rwai) :  
        ORIGIN = 0x004E0000,
        LENGTH = 0x00100000




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
    .gdt :
    {
        bin/sys_tables/gdt.elf	(.sys_tables);
    } > ram_sys_tables AT > rom_code






    .kernel_init32 :
    {
        *(.init32);
    } > ram_init32 AT > rom_code






    .isr :
    {
        *(.isr);
    } > ram_handlers AT > rom_code






    .sys_tables :
    {
        *(.sys_tables);
    } > ram_sys_tables AT > rom_code






    .kernel32 :
    {
        *(.kernel32);
        *(.funciones);
    } > ram_kernel32 AT > rom_code






    .tabla_digitos :
    {
        *(.tabla_digitos);
    } > ram_tabla_digitos AT > rom_code






    .tarea1 :
    {
        *(.tarea1);
    } > ram_tarea_1 AT > rom_code






    .my_data :
    {
        *(.my_bss);
        *(.my_data);
    } > ram_datos AT > rom_code






    .keyboard_buffer :
    {
        *(.keyboard_buffer);
    } > ram_keyboard_buffer AT > rom_code






    .init16 :
    {
        *(.init16);
        *(.gdtr16);
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
__STACK_SEG_16 	= 0x9000 	& 0xFFFF;
__STACK_END_16 	= 0xFFFF 	& 0xFFFF;

__STACK_SIZE_32 = LENGTH(ram_stack) / 4;
__STACK_END_32 	= ORIGIN(ram_stack) + LENGTH(ram_stack);

__SHDW_GDT16_SEG = (0x0000) 		& 0xFFFF;
__SHDW_GDT16_LEN = SIZEOF(.gdt) 	& 0xFFFF;
__SHDW_GDT16_ORG = LOADADDR(.gdt) 	& 0xFFFF;
__SHDW_GDT16_DST = (0x0000) 		& 0x0001;

__SHDW_INIT32_SEG = (ADDR(.kernel_init32) >> 16) 	& 0x000F;
__SHDW_INIT32_LEN = SIZEOF(.kernel_init32) 			& 0xFFFF;
__SHDW_INIT32_ORG = LOADADDR(.kernel_init32) 		& 0xFFFF;
__SHDW_INIT32_DST = ADDR(.kernel_init32) 			& 0xFFFF;

__SHDW_KERNEL32_LEN = SIZEOF(.kernel32);
__SHDW_KERNEL32_ORG = LOADADDR(.kernel32);
__SHDW_KERNEL32_DST = ADDR(.kernel32);

__SHDW_ISR_LEN = SIZEOF(.isr);
__SHDW_ISR_ORG = LOADADDR(.isr);
__SHDW_ISR_DST = ADDR(.isr);

__SHDW_SYS_TABLES_LEN = SIZEOF(.sys_tables);
__SHDW_SYS_TABLES_ORG = LOADADDR(.sys_tables);
__SHDW_SYS_TABLES_DST = ADDR(.sys_tables);

__SHDW_GDT_LEN = SIZEOF(.gdt);
__SHDW_GDT_ORG = LOADADDR(.gdt);
__SHDW_GDT_DST = ADDR(.gdt);

__SHDW_DATA_LEN = SIZEOF(.my_data);
__SHDW_DATA_ORG = LOADADDR(.my_data);
__SHDW_DATA_DST = ADDR(.my_data);

__SHDW_TAREA1_LEN = SIZEOF(.tarea1);
__SHDW_TAREA1_ORG = LOADADDR(.tarea1);
__SHDW_TAREA1_DST = ADDR(.tarea1);