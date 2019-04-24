; Funciones para habilitar el A20 Gate
%define     PORT_A_8042    0x60        ;Puerto A de E/S del 8042
%define     CTRL_PORT_8042 0x64        ;Puerto de Estado del 8042
%define     KEYB_DIS       0xAD        ;Deshabilita teclado con Command Byte
%define     KEYB_EN        0xAE        ;Habilita teclado con Command Byte
%define     READ_OUT_8042  0xD0        ;Copia en 0x60 el estado de OUT
%define     WRITE_OUT_8042 0xD1        ;Escribe en OUT lo almacenado en 0x60