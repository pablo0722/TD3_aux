; Funciones para habilitar el A20 Gate
%define     PORT_A_8042    0x60        ; Data Port: Puerto A de E/S del 8042
%define     CTRL_PORT_8042 0x64        ; Status Register (read), Command Register (write): Puerto de Estado del 8042
%define     KEYB_DIS       0xAD        ; Deshabilita teclado con Command Byte
%define     KEYB_EN        0xAE        ; Habilita teclado con Command Byte
%define     READ_OUT_8042  0xD0        ; Copia en 0x60 el estado de OUT
%define     WRITE_OUT_8042 0xD1        ; Escribe en OUT lo almacenado en 0x60

%define		KEY_ESC			0x01
%define		KEY_1 			0x02
%define		KEY_2 			0x03
%define		KEY_3 			0x04
%define		KEY_4 			0x05
%define		KEY_5 			0x06
%define		KEY_6 			0x07
%define		KEY_7 			0x08
%define		KEY_8 			0x09
%define		KEY_9 			0x0A
%define		KEY_0 			0x0B
%define		KEY_Q 			0x10
%define		KEY_W 			0x11
%define		KEY_E 			0x12
%define		KEY_R 			0x13
%define		KEY_T 			0x14
%define		KEY_Y 			0x15
%define		KEY_U 			0x16
%define		KEY_I 			0x17
%define		KEY_O 			0x18
%define		KEY_P 			0x19
%define		KEY_A 			0x1E
%define		KEY_S 			0x1F
%define		KEY_D 			0x20
%define		KEY_F 			0x21
%define		KEY_G 			0x22
%define		KEY_H 			0x23
%define		KEY_J 			0x24
%define		KEY_K 			0x25
%define		KEY_L 			0x26
%define		KEY_Z 			0x2C
%define		KEY_X 			0x2D
%define		KEY_C 			0x2E
%define		KEY_V 			0x2F
%define		KEY_B 			0x30
%define		KEY_N 			0x31
%define		KEY_M 			0x32