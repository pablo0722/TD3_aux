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
extern dword __SHDW_TAREA1_DST;
extern dword __SHDW_TAREA1_ORG;
extern dword __SHDW_TAREA1_LEN;










/************************************************
 * 					DATA						*
 ************************************************/
RingBuffer 	ring_buffer_tabla_digitos 	__TABLA_DIGITOS__;
qword		accum_digits 				__DATA__ 			= 0;










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
	_ring_buffer_init(&ring_buffer_tabla_digitos);

	_memcpy(&__SHDW_TAREA1_DST, &__SHDW_TAREA1_ORG, (dword) &__SHDW_TAREA1_LEN);
}










__TAREA1__ void _tarea1()
{
	_get_hexa_keys();
	_accum_digits();
}










__TAREA1__ static void _get_hexa_keys()
{
	static 	dword 	i 		__DATA__ 	= 0;
	static 	dword 	digito 	__DATA__ 	= 0;
			dword 	valor 				= -1;
			dword 	key;



	if(_ring_buffer_pop(&ring_buffer_keyboard, &key, 1) == 0)
	{
		if (i < 8)
		{
			if 		(key == KEY_0 || key == KEY_0_PAD)
			{
				valor = 0;
			}
			else if (key == KEY_1 || key == KEY_1_PAD)
			{
				valor = 1;
			}
			else if (key == KEY_2 || key == KEY_2_PAD)
			{
				valor = 2;
			}
			else if (key == KEY_3 || key == KEY_3_PAD)
			{
				valor = 3;
			}
			else if (key == KEY_4 || key == KEY_4_PAD)
			{
				valor = 4;
			}
			else if (key == KEY_5 || key == KEY_5_PAD)
			{
				valor = 5;
			}
			else if (key == KEY_6 || key == KEY_6_PAD)
			{
				valor = 6;
			}
			else if (key == KEY_7 || key == KEY_7_PAD)
			{
				valor = 7;
			}
			else if (key == KEY_8 || key == KEY_8_PAD)
			{
				valor = 8;
			}
			else if (key == KEY_9 || key == KEY_9_PAD)
			{
				valor = 9;
			}
			else if (key == KEY_A)
			{
				valor = 10;
			}
			else if (key == KEY_B)
			{
				valor = 11;
			}
			else if (key == KEY_C)
			{
				valor = 12;
			}
			else if (key == KEY_D)
			{
				valor = 13;
			}
			else if (key == KEY_E)
			{
				valor = 14;
			}
			else if (key == KEY_F)
			{
				valor = 15;
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










__TAREA1__ static void _accum_digits()
{
	dword 	digito;

	if(_ring_buffer_pop(&ring_buffer_tabla_digitos, &digito, 1) == 0)
	{
		accum_digits += digito;
	}
}