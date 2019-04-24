/*

											TIMER CADA 10 MS

╔═══════════════════╦════════════╦══════════════════════════════════════╦═══════════════════════════════════════════════════╗
║                   ║            ║              Dir Fisica              ║                                                   ║
║     Region        ║ Dir Lineal ╠════════════╦════════════╦════════════╣                Observaciones                      ║
║                   ║            ║  Tarea 0   ║  Tarea 1   ║  Tarea 2   ║                                                   ║
╠═══════════════════╬════════════╬════════════╬════════════╬════════════╬═══════════════════════════════════════════════════╣
║ ISR               ║ 0x00000000 ║ 0x00000000 ║ 0x00000000 ║ 0x00000000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ VGA               ║ 0x000B8000 ║ 0x000B8000 ║ 0x000B8000 ║ 0x000B8000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ SYS_TABLES        ║ 0x00100000 ║ 0x00100000 ║ 0x00100000 ║ 0x00100000 ║        Aca tambien estan los task_status_t        ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ PAG_TABLES        ║ 0x00110000 ║ 0x00110000 ║ 0x00191000 ║ 0x00212000 ║                                                   ║
║ PAG_TABLES        ║    ...     ║    ...     ║    ...     ║    ...     ║                                                   ║
║ PAG_TABLES        ║ 0x00190000 ║ 0x00190000 ║ 0x00211000 ║ 0x00292000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ KERNEL INIT/MAIN  ║ 0x00300000 ║ 0x00300000 ║ 0x00300000 ║ 0x00300000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ DIGIT_TABLES      ║ 0x00310000 ║ 0x00310000 ║ 0x00310000 ║ 0x00310000 ║                                                   ║
║ DIGIT_TABLES      ║    ...     ║    ...     ║    ...     ║    ...     ║                                                   ║
║ DIGIT_TABLES      ║ 0x003E0000 ║ 0x003E0000 ║ 0x003E0000 ║ 0x003E0000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ TEXT              ║ 0x00410000 ║ 0x00301000 ║ 0x00321000 ║ 0x00331000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ BSS               ║ 0x00411000 ║ 0x00302000 ║ 0x00322000 ║ 0x00332000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ DATA              ║ 0x00412000 ║ 0x00303000 ║ 0x00323000 ║ 0x00333000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ STACK NIVEL 3     ║ 0x00413000 ║ 0x1FFFC000 ║ 0x1FFFD000 ║ 0x1FFFE000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ STACK NIVEL 0     ║ 0x00414000 ║ 0x1FFF8000 ║ 0x1FFF9000 ║ 0x1FFFA000 ║ Aca tambien estan los tss_custom_t de cada tarea  ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ KEYBOARD BUFFER   ║ 0x004E0000 ║ 0x004E0000 ║ 0x004E0000 ║ 0x004E0000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ KERNEL FUNC       ║ 0x005E0000 ║ 0x005E0000 ║ 0x005E0000 ║ 0x005E0000 ║                                                   ║
║ KERNEL FUNC       ║ 0x005E1000 ║ 0x005E1000 ║ 0x005E1000 ║ 0x005E1000 ║                                                   ║
╠───────────────────╬────────────╬────────────╬────────────╬────────────╬───────────────────────────────────────────────────╣
║ STACK KERNEL      ║ 0x1FFFB000 ║ 0x1FFFB000 ║ 0x1FFFB000 ║ 0x1FFFB000 ║                                                   ║
║ STACK KERNEL      ║    ...     ║    ...     ║    ...     ║    ...     ║       Aca tambien esta el unico tss_intel_t       ║
║ STACK KERNEL      ║ 0x1FFFD000 ║ 0x1FFFD000 ║ 0x1FFFD000 ║ 0x1FFFD000 ║                                                   ║
╚═══════════════════╩════════════╩════════════╩════════════╩════════════╩═══════════════════════════════════════════════════╝
*/

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
        LENGTH = 0x000B8000





    ram_frame_buffer (rwxai) :  
        ORIGIN = 0x000B8000,
        LENGTH = 0x00048000





    ram_sys_tables (rwai) :  
        ORIGIN = 0x00100000,
        LENGTH = 0x00010000





    ram_pag_tables (rwai) :  
        ORIGIN = 0x00110000,
        LENGTH = 0x001F0000





    ram_kernel32 (rwxai) :  
        ORIGIN = 0x00300000,
        LENGTH = 0x00001000





    ram_tabla_digitos (rwai) :  
        ORIGIN = 0x00310000,
        LENGTH = 0x000D0000




    ram_datos (rwai) :
        ORIGIN = 0x003E0000,
        LENGTH = 0x00030000





    ram_tarea_text (rwai) :  
        ORIGIN = 0x00410000,
        LENGTH = 0x00001000





    ram_tarea_bss (rwai) :  
        ORIGIN = 0x00411000,
        LENGTH = 0x00001000





    ram_tarea_data (rwai) :
        ORIGIN = 0x00412000,
        LENGTH = 0x00001000





    ram_tarea_stack_nivel3 (rwai) :
        ORIGIN = 0x00413000,
        LENGTH = 0x00001000





    ram_tarea_stack_nivel0 (rwai) :
        ORIGIN = 0x00414000,
        LENGTH = 0x00001000




    ram_keyboard_buffer (rwai) :
        ORIGIN = 0x004E0000,
        LENGTH = 0x00100000





    ram_func32 (rwxai) :  
        ORIGIN = 0x005E0000,
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
        * 								(.init32);
    } > ram_init32 AT > rom_code






    .isr :
    {
        * 					(.isr);
        bin/isr/isr.c.elf	(.rodata);
    } > ram_handlers AT > rom_code






    .sys_tables :
    {
        *(.sys_tables);
        bin/sys_tables/pdt.c.elf		(.rodata);
    } > ram_sys_tables AT > rom_code






    .pag_tables :
    {
        *(.pag_tables);
    } > ram_pag_tables AT > rom_code






    .kernel32 :
    {
        *								(.kernel32);
    } > ram_kernel32 AT > rom_code






    .func32 :
    {
        *								(.funciones);
        bin/funciones/screen.c.elf		(.rodata);
        bin/kernel/kernel.c.elf			(.rodata);
        bin/kernel/kernel_init.c.elf	(.rodata);
    } > ram_func32 AT > rom_code






    .tabla_digitos :
    {
        *(.tabla_digitos);
    } > ram_tabla_digitos AT > rom_code






    OVERLAY :
    {
	    .tarea0_text
	    {
	        bin/tareas/tarea0.c.elf	(.text);
	        *(.tarea0);
	        bin/tareas/tarea0.c.elf	(.rodata);
	    }

	    .tarea1_text
	    {
	        bin/tareas/tarea1.c.elf	(.text);
	        *(.tarea1);
	        bin/tareas/tarea1.c.elf	(.rodata);
	    }

	    .tarea2_text
	    {
	        bin/tareas/tarea2.c.elf	(.text);
	        *(.tarea2);
	        bin/tareas/tarea2.c.elf	(.rodata);
	    }
	} > ram_tarea_text AT > rom_code






    OVERLAY :
    {
	    .tarea0_bss
	    {
	        bin/tareas/tarea0.c.elf		(.bss);
	    }

	    .tarea1_bss
	    {
	        bin/tareas/tarea1.c.elf		(.bss);
	    }

	    .tarea2_bss
	    {
	        bin/tareas/tarea2.c.elf		(.bss);
	    }
	} > ram_tarea_bss





    OVERLAY :
    {
    	.tarea0_data
        {
            bin/tareas/tarea0.c.elf	(.data);
            bin/tareas/tarea0.c.elf	(COMMON);
            bin/tareas/tarea0.c.elf	(.local);
            bin/tareas/tarea0.c.elf	(.comment);
            bin/tareas/tarea0.c.elf	(*);
        }

    	.tarea1_data
        {
            bin/tareas/tarea1.c.elf	(.data);
            bin/tareas/tarea1.c.elf	(COMMON);
            bin/tareas/tarea1.c.elf	(.local);
            bin/tareas/tarea1.c.elf	(.comment);
            bin/tareas/tarea1.c.elf	(*);
        }

    	.tarea2_data
        {
            bin/tareas/tarea2.c.elf	(.data);
            bin/tareas/tarea2.c.elf	(COMMON);
            bin/tareas/tarea2.c.elf	(.local);
            bin/tareas/tarea2.c.elf	(.comment);
            bin/tareas/tarea2.c.elf	(*);
        }
    } > ram_tarea_data AT > rom_code





    OVERLAY :
    {
	    .tarea0_stack_nivel3
	    {
	    }

	    .tarea1_stack_nivel3
	    {
	    }

	    .tarea2_stack_nivel3
	    {
	    }
	} > ram_tarea_stack_nivel3





    OVERLAY :
    {
	    .tarea0_stack_nivel0
	    {
	    	* (.bss.tss_tarea0);
	    }

	    .tarea1_stack_nivel0
	    {
	    	* (.bss.tss_tarea1);
	    }

	    .tarea2_stack_nivel0
	    {
	    	* (.bss.tss_tarea2);
	    }
	} > ram_tarea_stack_nivel0 AT > rom_code




	.tss_global :
	{
		* (.bss.tss_global);
	} > ram_stack AT > rom_code






    .my_data :
    {
        *(.my_bss);
        *(.my_data);
        bin/funciones/kmalloc.c.elf	(.data);
	    bin/funciones/screen.c.elf	(.bss);
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
__RAM_LEN = 0x20000000; /* 512MB */

__STACK_SEG_16 	= (0x9000) 	& 0xFFFF;
__STACK_END_16 	= (0xFFFF) 	& 0xFFFF;

__STACK_START_32 = ORIGIN(ram_stack);
__STACK_SIZE_32 = LENGTH(ram_stack) / 4;
__STACK_END_32 	= ORIGIN(ram_stack) + LENGTH(ram_stack);

__SHDW_GDT16_SEG = (0x0000) 		& 0xFFFF;
__SHDW_GDT16_LEN = SIZEOF(.gdt) 	& 0xFFFF;
__SHDW_GDT16_LMA = LOADADDR(.gdt) 	& 0xFFFF;
__SHDW_GDT16_LIN = (0x0000) 		& 0x0001;
__SHDW_GDT16_PHY = (0x0000) 		& 0x0001;

__SHDW_INIT32_SEG = (ADDR(.kernel_init32) >> 16) 	& 0x000F;
__SHDW_INIT32_LEN = SIZEOF(.kernel_init32) 			& 0xFFFF;
__SHDW_INIT32_LMA = LOADADDR(.kernel_init32) 		& 0xFFFF;
__SHDW_INIT32_LIN = ADDR(.kernel_init32) 			& 0xFFFF;
__SHDW_INIT32_PHY = ADDR(.kernel_init32) 			& 0xFFFF;

__SHDW_KERNEL32_LEN = SIZEOF(.kernel32);
__SHDW_KERNEL32_LMA = LOADADDR(.kernel32);
__SHDW_KERNEL32_LIN = ADDR(.kernel32);
__SHDW_KERNEL32_PHY = ADDR(.kernel32);

__SHDW_FUNC32_LEN = SIZEOF(.func32);
__SHDW_FUNC32_LMA = LOADADDR(.func32);
__SHDW_FUNC32_LIN = ADDR(.func32);
__SHDW_FUNC32_PHY = ADDR(.func32);

__SHDW_ISR_LEN = SIZEOF(.isr);
__SHDW_ISR_LMA = LOADADDR(.isr);
__SHDW_ISR_LIN = ADDR(.isr);
__SHDW_ISR_PHY = ADDR(.isr);

__SHDW_SYS_TABLES_LEN = SIZEOF(.sys_tables);
__SHDW_SYS_TABLES_LMA = LOADADDR(.sys_tables);
__SHDW_SYS_TABLES_LIN = ADDR(.sys_tables);
__SHDW_SYS_TABLES_PHY = ADDR(.sys_tables);

__SHDW_GDT_LEN = SIZEOF(.gdt);
__SHDW_GDT_LMA = LOADADDR(.gdt);
__SHDW_GDT_LIN = ADDR(.gdt);
__SHDW_GDT_PHY = ADDR(.gdt);

__SHDW_DATA_LEN = SIZEOF(.my_data);
__SHDW_DATA_LMA = LOADADDR(.my_data);
__SHDW_DATA_LIN = ADDR(.my_data);
__SHDW_DATA_PHY = ADDR(.my_data);

__TAREA0_TEXT_LMA = __load_start_tarea0_text;
__TAREA1_TEXT_LMA = __load_start_tarea1_text;
__TAREA2_TEXT_LMA = __load_start_tarea2_text;
__TAREA0_TEXT_LEN = __load_stop_tarea0_text - __load_start_tarea0_text;
__TAREA1_TEXT_LEN = __load_stop_tarea1_text - __load_start_tarea1_text;
__TAREA2_TEXT_LEN = __load_stop_tarea2_text - __load_start_tarea2_text;
__TAREA0_TEXT_PHY = 0x00301000;
__TAREA1_TEXT_PHY = 0x00321000;
__TAREA2_TEXT_PHY = 0x00331000;
__TAREA_TEXT_LIN = ORIGIN(ram_tarea_text);

__TAREA0_BSS_PHY = 0x00302000;
__TAREA1_BSS_PHY = 0x00322000;
__TAREA2_BSS_PHY = 0x00332000;
__TAREA0_BSS_LEN = 0x1000;
__TAREA1_BSS_LEN = 0x1000;
__TAREA2_BSS_LEN = 0x1000;
__TAREA_BSS_LIN = ORIGIN(ram_tarea_bss);

__TAREA0_DATA_LMA = __load_start_tarea0_data;
__TAREA1_DATA_LMA = __load_start_tarea1_data;
__TAREA2_DATA_LMA = __load_start_tarea2_data;
__TAREA0_DATA_LEN = __load_stop_tarea0_data - __load_start_tarea0_data;
__TAREA1_DATA_LEN = __load_stop_tarea1_data - __load_start_tarea1_data;
__TAREA2_DATA_LEN = __load_stop_tarea2_data - __load_start_tarea2_data;
__TAREA0_DATA_PHY = 0x00303000;
__TAREA1_DATA_PHY = 0x00323000;
__TAREA2_DATA_PHY = 0x00333000;
__TAREA_DATA_LIN = ORIGIN(ram_tarea_data);

__STACK_TAREA_NIVEL3_LIN 	= ORIGIN(ram_tarea_stack_nivel3);
__STACK_TAREA_NIVEL3_LEN 	= LENGTH(ram_tarea_stack_nivel3) / 4;
__STACK_TAREA_NIVEL3_END 	= ORIGIN(ram_tarea_stack_nivel3) + LENGTH(ram_tarea_stack_nivel3);
__STACK_TAREA0_NIVEL3_PHY 	= 0x1FFFC000;
__STACK_TAREA1_NIVEL3_PHY 	= 0x1FFFD000;
__STACK_TAREA2_NIVEL3_PHY 	= 0x1FFFE000;

__STACK_TAREA_NIVEL0_LIN 		= ORIGIN(ram_tarea_stack_nivel0);
__STACK_TAREA_NIVEL0_LEN 		= LENGTH(ram_tarea_stack_nivel0) / 4;
__STACK_TAREA_NIVEL0_LIN_END 	= ORIGIN(ram_tarea_stack_nivel0) + LENGTH(ram_tarea_stack_nivel0);
__STACK_TAREA0_NIVEL0_LMA 		= __load_start_tarea0_stack_nivel0;
__STACK_TAREA1_NIVEL0_LMA 		= __load_start_tarea1_stack_nivel0;
__STACK_TAREA2_NIVEL0_LMA 		= __load_start_tarea2_stack_nivel0;
__STACK_TAREA0_NIVEL0_PHY 		= 0x1FFF8000;
__STACK_TAREA1_NIVEL0_PHY 		= 0x1FFF9000;
__STACK_TAREA2_NIVEL0_PHY 		= 0x1FFFA000;

__TSS_GLOBAL_LIN 		= ADDR(.tss_global);
__TSS_GLOBAL_LIN_00_15 	=  __TSS_GLOBAL_LIN  		& 0xFFFF;
__TSS_GLOBAL_LIN_16_23 	= (__TSS_GLOBAL_LIN >> 16)  & 0xFF;
__TSS_GLOBAL_LIN_24_31 	= (__TSS_GLOBAL_LIN >> 24)  & 0xFF;
__TSS_GLOBAL_LMA 		= LOADADDR(.tss_global);
__TSS_GLOBAL_LEN 		= SIZEOF(.tss_global);
__TSS_GLOBAL_PHY 		= ADDR(.tss_global);

__TABLA_DIGITOS_LIN = ORIGIN(ram_tabla_digitos);
__TABLA_DIGITOS_PHY = ORIGIN(ram_tabla_digitos);
__TABLA_DIGITOS_LEN = LENGTH(ram_tabla_digitos);

__KEYBOARD_BUFFER_LIN = ADDR(.keyboard_buffer);
__KEYBOARD_BUFFER_PHY = ADDR(.keyboard_buffer);
__KEYBOARD_BUFFER_LEN = SIZEOF(.keyboard_buffer);

__PDT_TAREA_LEN = ((__RAM_LEN >> 22) + 1)*1024*4;
__PDT_TAREA0_LEN = __PDT_TAREA_LEN*0;
__PDT_TAREA1_LEN = __PDT_TAREA_LEN*1;
__PDT_TAREA2_LEN = __PDT_TAREA_LEN*2;
__PDT_TAREA0_LIN = ORIGIN(ram_pag_tables) + __PDT_TAREA0_LEN;
__PDT_TAREA1_LIN = __PDT_TAREA0_LIN;
__PDT_TAREA2_LIN = __PDT_TAREA0_LIN;
__PDT_TAREA0_PHY = ORIGIN(ram_pag_tables) + __PDT_TAREA0_LEN;
__PDT_TAREA1_PHY = ORIGIN(ram_pag_tables) + __PDT_TAREA1_LEN;
__PDT_TAREA2_PHY = ORIGIN(ram_pag_tables) + __PDT_TAREA2_LEN;