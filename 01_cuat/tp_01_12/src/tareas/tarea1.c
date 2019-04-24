/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "sys_tables/sys_tables.h"
#include "funciones/funciones.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DEFINES LINKER				*
 ************************************************/
extern dword __SHDW_TAREA1_TEXT_DST;
extern dword __SHDW_TAREA1_TEXT_PHY;
extern dword __SHDW_TAREA1_TEXT_ORG;
extern dword __SHDW_TAREA1_TEXT_LEN;

extern dword __SHDW_TAREA1_BSS_DST;
extern dword __SHDW_TAREA1_BSS_PHY;
extern dword __SHDW_TAREA1_BSS_LEN;

extern dword __SHDW_TAREA1_DATA_DST;
extern dword __SHDW_TAREA1_DATA_PHY;
extern dword __SHDW_TAREA1_DATA_ORG;
extern dword __SHDW_TAREA1_DATA_LEN;










/************************************************
 * 					DATA						*
 ************************************************/
RingBuffer 	ring_buffer_tabla_digitos 	__TABLA_DIGITOS__;
byte *		accum_digits 				__BSS__;










/************************************************
 * 					LOCAL FORWARDS				*
 ************************************************/
__KERNEL__ static void _get_hexa_keys();
__KERNEL__ static void _accum_digits();










/************************************************
 * 					EXCEPTION ISR 				*
 ************************************************/
__KERNEL__ void _tarea1_init()
{
	screen_write_str("_tarea1_init\n");
	
	_memcpy(&__SHDW_TAREA1_TEXT_PHY, 	&__SHDW_TAREA1_TEXT_ORG, 	(dword) &__SHDW_TAREA1_TEXT_LEN);
	_memcpy(&__SHDW_TAREA1_DATA_PHY, 	&__SHDW_TAREA1_DATA_ORG, 	(dword) &__SHDW_TAREA1_DATA_LEN);

	_ring_buffer_init(&ring_buffer_tabla_digitos);
}










__TEXT__ void _tarea1()
{
	_get_hexa_keys();
	_accum_digits();
}










__TEXT__ static void _get_hexa_keys()
{
	static 	dword 	i 		__BSS__ = 0;
	static 	dword 	digito 	__BSS__ = 0;
			dword 	valor 			= -1;



	if(key != 0)
	{
		if (i < 8)
		{
			switch(key)
			{
				case KEY_0:
				case KEY_0_PAD:
					valor = 0;
					break;

				case KEY_1:
				case KEY_1_PAD:
					valor = 1;
					break;

				case KEY_2:
				case KEY_2_PAD:
					valor = 2;
					break;

				case KEY_3:
				case KEY_3_PAD:
					valor = 3;
					break;

				case KEY_4:
				case KEY_4_PAD:
					valor = 4;
					break;

				case KEY_5:
				case KEY_5_PAD:
					valor = 5;
					break;

				case KEY_6:
				case KEY_6_PAD:
					valor = 6;
					break;

				case KEY_7:
				case KEY_7_PAD:
					valor = 7;
					break;

				case KEY_8:
				case KEY_8_PAD:
					valor = 8;
					break;

				case KEY_9:
				case KEY_9_PAD:
					valor = 9;
					break;

				case KEY_A:
					valor = 10;
					break;

				case KEY_B:
					valor = 11;
					break;

				case KEY_C:
					valor = 12;
					break;

				case KEY_D:
					valor = 13;
					break;

				case KEY_E:
					valor = 14;
					break;

				case KEY_F:
					valor = 15;
					break;
			}

			if(valor != -1)
			{

				digito = (digito << 4) | valor;
				i++;
				valor = -1;
			}
		}

		if (key == KEY_ENTER)
		{
			_ring_buffer_push(&ring_buffer_tabla_digitos, &digito, 1);
			
			digito = 0;
			i = 0;
		}
	}
}










__TEXT__ static void _accum_digits()
{
	dword 	digito;

	if(_ring_buffer_pop(&ring_buffer_tabla_digitos, &digito, 1) == 0)
	{
		accum_digits += digito;

		if((dword)(accum_digits+3) < RAM)
		{
			screen_write_str("leyendo en direccion ");
			screen_write_dword_hex((dword) accum_digits);

			screen_write_str(": ");
			screen_write_dword_hex(*((dword *)accum_digits));
		}
		else
		{
			screen_write_str("(omitiendo lectura) accum_digits: ");
			screen_write_dword_hex((dword) accum_digits);
		}

		screen_write_str("\n");
	}
}