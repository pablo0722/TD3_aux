;********************************************************
;						DIRECTIVAS						*
;********************************************************
SECTION .init16

GLOBAL _invalide_tlb
GLOBAL _a20_enable
GLOBAL _cacheDisable
GLOBAL _timer_init
GLOBAL _rtc_init
GLOBAL _pic_init

USE16










;********************************************************
;						INCLUDES						*
;********************************************************
%include "init16_32/header16.inc"










;********************************************************
;						FUNCIONES						*
;********************************************************
; void _invalide_tlb();
_invalide_tlb:
	.LFB:
		xor eax, eax 	; ax = 0
		mov cr3, eax 	;Invalidar TLB

		ret
	.LFE:










; void _a20_enable();
_a20_enable:
	.LBF0:			; Habilita el A20 gate sin utilizacion del stack.
		;Deshabilita el teclado
		mov di, .8042_kbrd_dis
		jmp .empty_8042_in
	.8042_kbrd_dis:
		mov al, KEYB_DIS
		out CTRL_PORT_8042, al

		;Lee la salida
		mov di, .8042_read_out
		jmp .empty_8042_in
	.8042_read_out:
		mov al, READ_OUT_8042
		out CTRL_PORT_8042, al

	.empty_8042_out:  
		;      in al, CTRL_PORT_8042      ; Lee port de estado del 8042 hasta que el
		;      test al, 00000001b         ; buffer de salida este vacio
		;      jne .empty_8042_out

		xor bx, bx   
		in al, PORT_A_8042
		mov bx, ax

		;Modifica el valor del A20
		mov di, .8042_write_out
		jmp .empty_8042_in
	.8042_write_out:
		mov al, WRITE_OUT_8042
		out CTRL_PORT_8042, al

		mov di, .8042_set_a20
		jmp .empty_8042_in
	.8042_set_a20:
		mov ax, bx
		or ax, 00000010b              ; Habilita el bit A20
		out PORT_A_8042, al

		;Habilita el teclado
		mov di, .8042_kbrd_en
		jmp .empty_8042_in
	.8042_kbrd_en:
		mov al, KEYB_EN
		out CTRL_PORT_8042, al

		mov di, .LFE
	.empty_8042_in:  
		;      in al, CTRL_PORT_8042      ; Lee port de estado del 8042 hasta que el
		;      test al, 00000010b         ; buffer de entrada este vacio
		;      jne .empty_8042_in
		jmp di
	.LFE:










; void _cacheDisable();
_cacheDisable:
	.LFB:
		mov   eax, cr0
		or    eax, (X86_CR0_NW | X86_CR0_CD)
		mov   cr0, eax
		wbinvd
	.LFE:










;-------------------------------------------------------------------------------
;|  Título:         PIT_Set_Counter0                                           |
;|  Versión:        1.0                     Fecha:  24/08/2014                 |
;|  Autor:          Andrea Pirlo            Modelo: IA-32 (16/32bits)          |
;|  ------------------------------------------------------------------------   |
;|  Descripción:                                                               |
;|      Reprograma el Temporizador 0 del PIT (Programmable Interval Timer)     |
;|  ------------------------------------------------------------------------   |
;|  Recibe:                                                                    |
;|      cx:     Periodo de interrupcion expresado en multiplos de 1mseg.       |
;|              Debe ser inferior a 54.                                        |
;|                                                                             |
;|  Retorna:                                                                   |
;|      Nada                                                                   |
;|  ------------------------------------------------------------------------   |
;|  Revisiones:                                                                |
;|      1.0 | 15/02/2010 | D.GARCIA  | Original                                |
;|      2.0 | 01/06/2017 | ChristiaN | Se agrega argumento para intervalo      |
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
;7 6 5 4 3 2 1 0   <-- Número de bit de la palabra de control
;| | | | | | | |
;| | | | | | | +-- Modo BCD:
;| | | | | | |     0 - El contador trabajará en formato binario de 16 bits
;| | | | | | |     1 - El contador trabajará en formato BCD con cuatro dígitos 
;| | | | | | |          decimales
;| | | | +-+-+---- Modo de operación para el contador:
;| | | |           000 - Modo 0. Interrupt on Terminal Count (Interrumpe al terminar el conteo)
;| | | |           001 - Modo 1. Hardware Retriggerable One-Shot (Disparo programable)
;| | | |           X10 - Modo 2. Rate Generator (Generador de impulsos). El valor del bit más significativo no importa
;| | | |           X11 - Modo 3. Square Wave(Generador de onda cuadrada). El valor del bit más significativo no importa
;| | | |           100 - Modo 4. Software Triggered Strobe (Strobe disparado por software)
;| | | |           101 - Modo 5. Hardware Triggered Strobe (Retriggerable) (Strobe disparado por hardware)
;| | | |
;| | +-+---------- Modo de acceso (lectura/escritura) para el valor del contador:
;| |               00 - Counter Latch. El valor puede ser leído de la manera en que fue ajustado previamente.
;| |                                   El valor es mantenido hasta que es leído o sobreescrito.
;| |               01 - Lee (o escribe) solo el byte menos significativo del contador (bits 0-7)
;| |               10 - Lee (o escribe) solo el byte más significativo del contador (bits 8-15)
;| |               11 - Primero se lee (o escribe) el byte menos significativo del contador, y luego el byte más significativo
;| |
;+-+-------------- Selección del contador:
;                  00 - Se selecciona el contador 0
;                  01 - Se selecciona el contador 1
;                  10 - Se selecciona el contador 2
;                  11 - No usado. (solo hay 3 contadores)
;                  (Los demás bits de la palabra de control indican cómo será programado el contador seleccionado)


; void _timer_init();
_timer_init:
	.LFB:
		; Intervalo maximo: 54 mseg
		mov ecx, 10 		;Interrumpir cada 10mseg

		mov al, 00110100b
		out 43h, al 		;En 43h está el registro de control.

		mov eax, 1193182 	;Los 3 contadores del PIT reciben una señal de clock de 1,193182[MHz] 
		mul ecx 			;1193182 * cx / 1000 = cuentas

		mov ecx, 1000
		div ecx
		out 40h, al 		; En 40h está el Counter 0.
		mov al, ah
		out 40h, al

	    ret
	.LFE:










;--------------------------------------------------------------------------------
;|  Título:         Control RTC                                                |
;|  Versión:        1.0                     Fecha:  16/07/2009                 |
;|  Autor:          D.GARCIA                Modelo: IA-32 (32 bits)            |
;|  ------------------------------------------------------------------------   |
;|  Descripción:                                                               |
;|      Rutina para manejo de servicios del Reloj de Tiempo Real               |
;|  ------------------------------------------------------------------------   |
;|  Recibe:                                                                    |
;|      AL = 0 Subfuncion fecha                                                |
;|      AL = 1 Subfuncion hora                                                 |
;|                                                                             |
;|  Retorna:                                                                   |
;|      Fecha:                                                                 |
;|          DH = Año                                                           |
;|          DL = Mes                                                           |
;|          AH = Dia                                                           |
;|          AL = Dia de la semana                                              |
;|      Hora:                                                                  |
;|          DL = Hora                                                          |
;|          AH = Minutos                                                       |
;|          AL = Segundos                                                      |
;|          CL = 0:OK  N:Codigo de error                                       |
;|  ------------------------------------------------------------------------   |
;|  Revisiones:                                                                |
;|      1.0 | 15/02/2010 | D.GARCIA | Original                                 |
;-------------------------------------------------------------------------------
; void _rtc_init();
_rtc_init:
	.LFB:
		cmp     al, 0
		je      _rtc_fecha 			; Servicio de Fecha
		cmp     al, 1
		je      _rtc_hora 			; Servicio de Hora
		
		; Funcion no valida, salida con error
		mov     cl, 1 				; Codigo de error. Subfuncion invalida
		ret

	RTC_Exit:
		mov     cl, 0 				; Codigo de error. OK

		ret
	.LFE:










;-------------------------------------------------------------------------------
;|  Título:         Auxiliar RTC                                               |
;|  Versión:        1.0                     Fecha:  16/07/2009                 |
;|  Autor:          D.GARCIA                Modelo: IA-32 (32 bits)            |
;|  ------------------------------------------------------------------------   |
;|  Descripción:                                                               |
;|      Subfuncion para obtener la hora del sistema desde el RTC               |
;|  ------------------------------------------------------------------------   |
;|  Recibe:                                                                    |
;|      Nada                                                                   |
;|  Retorna:                                                                   |
;|      Nada                                                                   |
;|  ------------------------------------------------------------------------   |
;|  Revisiones:                                                                |
;|      1.0 | 15/02/2010 | D.GARCIA | Original                                 |
;-------------------------------------------------------------------------------
; void _rtc_hora();
_rtc_hora:
	.LFB:
	    call    _rtc_disponible 	; asegura que no está actualizándose el RTC
	    mov     al, 4
	    out     70h, al 			; Selecciona Registro de Hora
	    in      al, 71h 			; lee hora
	    mov     dl, al

	    mov     al, 2
	    out     70h, al 			; Selecciona Registro de Minutos
	    in      al, 71h 			; lee minutos
	    mov     ah, al

	    xor     al, al
	    out     70h, al 			; Selecciona Registro de Segundos
	    in      al, 71h 			; lee minutos

	    jmp     RTC_Exit
    .LFE:










;-------------------------------------------------------------------------------
;|  Título:         Auxiliar RTC                                               |
;|  Versión:        1.0                     Fecha:  16/07/2009                 |
;|  Autor:          D.GARCIA                Modelo: IA-32 (32 bits)            |
;|  ------------------------------------------------------------------------   |
;|  Descripción:                                                               |
;|      Subfuncion para obtener la fecha del sistema desde el RTC              |
;|  ------------------------------------------------------------------------   |
;|  Recibe:                                                                    |
;|      Nada                                                                   |
;|  Retorna:                                                                   |
;|      Nada                                                                   |
;|  ------------------------------------------------------------------------   |
;|  Revisiones:                                                                |
;|      1.0 | 15/02/2010 | D.GARCIA | Original                                 |
;-------------------------------------------------------------------------------
; void _rtc_fecha();
_rtc_fecha:
    call    _rtc_disponible 	; asegura que no esté 
 								; actualizandose el RTC
    mov     al, 9
    out     70h, al 			; Selecciona Registro de Año
    in      al, 71h 			; lee año 
    mov     dh, al

    mov     al, 8
    out     70h, al 			; Selecciona Registro de Mes
    in      al, 71h 			; lee mes
    mov     dl, al

    mov     al, 7
    out     70h, al 			; Selecciona Registro de Fecha
    in      al, 71h 			; lee Fecha del mes
    mov     ah, al

    mov     al, 6
    out     70h, al 			; Selecciona Registro de Día 
    in      al, 71h 			; lee día de la semana

    jmp     RTC_Exit










;-------------------------------------------------------------------------------
;|  Título:         Auxiliar RTC                                               |
;|  Versión:        1.0                     Fecha:  16/07/2009                 |
;|  Autor:          D.GARCIA                Modelo: IA-32e (64 bits)           |
;|  ------------------------------------------------------------------------   |
;|  Descripción:                                                               |
;|      Verifica en el Status Register A que el RTC no esta actualizando       |
;|      fecha y hora.                                                          |
;|      Retorna cuando el RTC esta disponible                                  |
;|  ------------------------------------------------------------------------   |
;|  Recibe:                                                                    |
;|      Nada                                                                   |
;|  Retorna:                                                                   |
;|      Nada                                                                   |
;|  ------------------------------------------------------------------------   |
;|  Revisiones:                                                                |
;|      1.0 | 15/02/2010 | D.GARCIA | Original                                 |
;-------------------------------------------------------------------------------
; void _rtc_disponible();
_rtc_disponible:
	.LFB:
		mov     al, 0Ah
		out     70h, al 		; Selecciona registro de status A

	.wait_for_free:
		in      al, 71h 		; lee Status
		test    al, 80h
		jnz     .wait_for_free

		ret
	.LFE:










; IRQ 0 ‒ system timer
; IRQ 1 — keyboard controller
; IRQ 3 — serial port COM2
; IRQ 4 — serial port COM1
; IRQ 5 — line print terminal 2
; IRQ 6 — floppy controller
; IRQ 7 — line print terminal 1
; IRQ 8 — RTC timer
; IRQ 12 — mouse controller
; IRQ 13 — math co-processor
; IRQ 14 — ATA channel 1
; IRQ 15 — ATA channel 2

; void _pic_init();
_pic_init:
	.LFB:
		mov bx, 0x2820 				; Base PIC0=0x20 PIC1=0x28

		mov 	al, 11h 			; ICW1: IRQs activas x flanco, cascada, e ICW4
		out 	20h, al  
		mov 	al, bl 				; ICW2: El PIC 1 arranca en INT tipo "bl"
		out 	21h, al
		mov 	al, 04h 			; ICW3: PIC 1 Master, Slave, Ingresa Int x IRQ2
		out 	21h,al
		mov 	al, 01h 			; ICW4: Modo 8086
		out 	21h, al
		mov 	al, 0FFh 			; Deshabilito las interrupciones del PIC 1 
		out 	21h, al
		mov 	al, 11h 			; ICW1: IRQs activas x flanco, cascada, e ICW4
		out 	0A0h, al  
		mov 	al, bh 				; ICW 2: El PIC 2 arranca en INT tipo "bh"
		out 	0A1h, al
		mov 	al, 02h 			; ICW 3: PIC 2 Slave, Ingresa Int x IRQ2
		out 	0A1h, al
		mov 	al, 01h 			; ICW 4: Modo 8086
		out 	0A1h, al
		mov 	al, 0FFh 			; Deshabilito las interrupciones del PIC 2
		out 	0A1h, al

		; Deshabilita todas las interrupciones del PIC
		mov 	al, 0xff			; Deshabilita todas las interrupciones del PIC 1
		out 	0x21, al			

		mov 	al, 0xff			; Deshabilita todas las interrupciones del PIC 2
		out 	0xA1, al			

		; Habilita Keyboard Controller (IRQ 1) 7 Timer (IRQ 0)
		in 		al, 0x21
		and 	al, ~(0x3<<0)
		out 	0x21, al

		ret
	.LFE: