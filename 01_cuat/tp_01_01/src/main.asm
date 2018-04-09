; Compilo con: nasm rom.s -o rom.bin -l rom.lst
; *.s es el codigo en asembler
; *.bin es el binario. Se puede leer con: hexdump *.bin
; *.lst lista las instrucciones en el formato: fila, direccion, opcode y mnemonico. Se puede leer con: cat *.lst

section .text
global _start

_start:
	.LFB0:
	etiqueta:  db 0x00 		; db define un byte en esta direccion (en la misma direccion de la instruccion). La "etiqueta:" es la direccion de "db", entonces "etiqueta" es = 0x00. Solo se usa a modo de ejemplo, no tiene utilidad en este codigo.
		times 65520 nop 	; Reserva 65519 bytes
		cli					; Deshabilita las interrupciones. Se asegura que nadie me interrumpa.
		cld					; Limpia flags de direccion. Se asegura que vaya en orden ascendente.
	STOP:
		hlt					; Detiene el procesador. Continua ejecutando solo en caso que llegue una interrupcion.
		jmp STOP			; Es como while(1){hlt} para que nunca salga del hlt
		align 16			; Alinea (rellena con NOP) hasta completar 16 bits
	.LFE0: