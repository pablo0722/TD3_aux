/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "funciones/funciones.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/










/************************************************
 * 					DATA						*
 ************************************************/
RingBuffer ring_buffer_tabla_digitos __TABLA_DIGITOS__;










/************************************************
 * 					LOCAL FORWARDS				*
 ************************************************/
__KERNEL__ static void _get_hexa_keys();










/************************************************
 * 					FUNCIONES 					*
 ************************************************/
__KERNEL__ void _kernel_main()
{
	dword tick_old = tick;

	_kernel_init();

	_ring_buffer_init(&ring_buffer_tabla_digitos);

	while(1)
	{
		if(tick < tick_old+1)
		{
			continue;
		}
		tick_old = tick;

		_get_hexa_keys();
	}

	_hlt();
}










__KERNEL__ static void _get_hexa_keys()
{
	static 	dword 	i 		__BSS__ 	= 0;
	static 	dword 	digito 	__BSS__ 	= 0;
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