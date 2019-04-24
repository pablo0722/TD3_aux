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
        LENGTH = 0x00010000





    ram_pag_tables (rwai) :  
        ORIGIN = 0x00110000,
        LENGTH = 0x001F0000





    ram_kernel32 (rwxai) :  
        ORIGIN = 0x00300000,
        LENGTH = 0x00010000





    ram_tabla_digitos (rwai) :  
        ORIGIN = 0x00310000,
        LENGTH = 0x000D0000




    ram_datos (rwai) :
        ORIGIN = 0x003E0000,
        LENGTH = 0x00100000





    ram_tarea1_text (rwai) :  
        ORIGIN = 0x00410000,
        LENGTH = 0x00001000





    ram_tarea1_bss (rwai) :  
        ORIGIN = 0x00411000,
        LENGTH = 0x00001000





    ram_tarea1_data (rwai) :
        ORIGIN = 0x00412000,
        LENGTH = 0x00001000





    ram_tarea1_stack (rwai) :
        ORIGIN = 0x00413000,
        LENGTH = 0x00002000




    ram_keyboard_buffer (rwai) :
        ORIGIN = 0x004E0000,
        LENGTH = 0x00100000




    ram_stack (rwai) :  
        ORIGIN = 0x1FFFB000,
        LENGTH = 0x00002000





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






    .pag_tables :
    {
        *(.pag_tables);
    } > ram_pag_tables AT > rom_code






    .kernel32 :
    {
        *(.kernel32);
        *(.funciones);
    } > ram_kernel32 AT > rom_code






    .tabla_digitos :
    {
        *(.tabla_digitos);
    } > ram_tabla_digitos AT > rom_code






    .tarea1_text :
    {
        *(.tarea1);
        bin/tareas/tarea1.c.elf	(.text);
        bin/tareas/tarea1.c.elf	(.rodata);
    } > ram_tarea1_text AT > rom_code






    .tarea1_bss :
    {
        bin/tareas/tarea1.c.elf	(.bss);
    } > ram_tarea1_bss






    .tarea1_data :
    {
        bin/tareas/tarea1.c.elf	(.data);
        bin/tareas/tarea1.c.elf	(COMMON);
        bin/tareas/tarea1.c.elf	(.local);
        bin/tareas/tarea1.c.elf	(.comment);
        bin/tareas/tarea1.c.elf	(*);
    } > ram_tarea1_data AT > rom_code






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






    .others :
    {
        *(.comment);
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

__STACK_START_32 = ORIGIN(ram_stack);
__STACK_SIZE_32 = LENGTH(ram_stack) / 4;
__STACK_END_32 	= ORIGIN(ram_stack) + LENGTH(ram_stack);

__STACK_TAREA1_LIN 	= ORIGIN(ram_tarea1_stack);
__STACK_TAREA1_SIZE = LENGTH(ram_tarea1_stack) / 4;
__STACK_TAREA1_END 	= ORIGIN(ram_tarea1_stack) + LENGTH(ram_tarea1_stack);
__STACK_TAREA1_PHY 	= 0x1FFFE000;

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

__SHDW_TAREA1_TEXT_LEN = SIZEOF(.tarea1_text);
__SHDW_TAREA1_TEXT_PHY = 0x00321000;
__SHDW_TAREA1_TEXT_ORG = LOADADDR(.tarea1_text);
__SHDW_TAREA1_TEXT_DST = ADDR(.tarea1_text);

__SHDW_TAREA1_BSS_LEN = SIZEOF(.tarea1_bss);
__SHDW_TAREA1_BSS_PHY = 0x00322000;
__SHDW_TAREA1_BSS_DST = ADDR(.tarea1_bss);

__SHDW_TAREA1_DATA_LEN = SIZEOF(.tarea1_data);
__SHDW_TAREA1_DATA_PHY = 0x00323000;
__SHDW_TAREA1_DATA_ORG = LOADADDR(.tarea1_data);
__SHDW_TAREA1_DATA_DST = ADDR(.tarea1_data);

__TABLA_DIGITOS_LIN = ORIGIN(ram_tabla_digitos);
__TABLA_DIGITOS_LEN = LENGTH(ram_tabla_digitos);

__KEYBOARD_BUFFER_LIN = ADDR(.keyboard_buffer);
__KEYBOARD_BUFFER_LEN = SIZEOF(.keyboard_buffer);

__PDT_START = ORIGIN(ram_pag_tables);
__PDT_LEN = LENGTH(ram_pag_tables);
__PDT_END = ORIGIN(ram_pag_tables) + LENGTH(ram_pag_tables);
