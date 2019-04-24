#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

/************************************************
 * 					INCLUDES 					*
 ************************************************/
#include "kernel/kernel.h"
#include "funciones/ring_buffer.h"










/************************************************
 * 					DEFINES	 					*
 ************************************************/
// SCAN CODES

// Utilizar con una OR (|)
#define 	KEY_PRESSED			0x00
#define 	KEY_RELEASED		0x80

// Teclas
#define		KEY_ESC				0x01

#define		KEY_MINUS			0x0C
#define		KEY_MINUS_PAD		0x4A
#define		KEY_PLUS_PAD		0x4E
#define		KEY_EQUAL			0x0D
#define		KEY_BACKSPACE		0x0E
#define		KEY_TAB				0x0F
#define		KEY_OPENBRACE		0x1A
#define		KEY_CLOSEBRACE		0x1B
#define		KEY_ENTER			0x1C
#define		KEY_LCTRL			0x1D
#define		KEY_SEMICOLON		0x27
#define		KEY_QUOTE			0x28
#define		KEY_BACKQUOTE		0x29
#define		KEY_LSHIFT			0x2A
#define		KEY_BACKSLASH		0x28
#define		KEY_COMMA			0x33
#define		KEY_STOP			0x34
#define		KEY_STOP_PAD		0x53
#define		KEY_SLASH			0x35
#define		KEY_RSHIFT			0x36
#define		KEY_ASTERISK		0x37
#define		KEY_ALT				0x38
#define		KEY_SPACE			0x39

#define		KEY_CAPSLOCK		0x3A
#define		KEY_NUMLOCK			0x45
#define		KEY_SCROLLLOCK		0x46

#define		KEY_F1				0x3B
#define		KEY_F2				0x3C
#define		KEY_F3				0x3D
#define		KEY_F4				0x3E
#define		KEY_F5				0x3F
#define		KEY_F6				0x40
#define		KEY_F7				0x41
#define		KEY_F8				0x42
#define		KEY_F9				0x43
#define		KEY_F10				0x44
#define		KEY_F11				0x57

#define		KEY_1 				0x02
#define		KEY_2 				0x03
#define		KEY_3 				0x04
#define		KEY_4 				0x05
#define		KEY_5 				0x06
#define		KEY_6 				0x07
#define		KEY_7 				0x08
#define		KEY_8 				0x09
#define		KEY_9 				0x0A
#define		KEY_0 				0x0B

#define		KEY_1_PAD			0x4F
#define		KEY_2_PAD			0x50
#define		KEY_3_PAD			0x51
#define		KEY_4_PAD			0x4B
#define		KEY_5_PAD			0x4C
#define		KEY_6_PAD			0x4D
#define		KEY_7_PAD			0x47
#define		KEY_8_PAD			0x48
#define		KEY_9_PAD			0x49
#define		KEY_0_PAD			0x52

#define		KEY_A 				0x1E
#define		KEY_B 				0x30
#define		KEY_C 				0x2E
#define		KEY_D 				0x20
#define		KEY_E 				0x12
#define		KEY_F 				0x21
#define		KEY_G 				0x22
#define		KEY_H 				0x23
#define		KEY_I 				0x17
#define		KEY_J 				0x24
#define		KEY_K 				0x25
#define		KEY_L 				0x26
#define		KEY_M 				0x32
#define		KEY_N 				0x31
#define		KEY_O 				0x18
#define		KEY_P 				0x19
#define		KEY_Q 				0x10
#define		KEY_R 				0x13
#define		KEY_S 				0x1F
#define		KEY_T 				0x14
#define		KEY_U 				0x16
#define		KEY_V 				0x2F
#define		KEY_W 				0x11
#define		KEY_X 				0x2D
#define		KEY_Y 				0x15
#define		KEY_Z 				0x2C

#define		KEY_EXTRA 			0xE0

// Si se detecta KEY_EXTRA, el significado de las teclas cambia.
// Ej: si se detecta 0x9D, es el Left_Control,
// Pero si primero se detecta 0xE0 y luego 0x9D, es el Right_Control


// EXTRA KEYS:
#define		KEY_UP 				0x48 	; EXTRA KEY
#define		KEY_LEFT			0x4B 	; EXTRA KEY
#define		KEY_RIGHT			0x4D 	; EXTRA KEY
#define		KEY_DOWN			0x50 	; EXTRA KEY

#define		KEY_ENTER_PAD 		0x1C 	; EXTRA KEY
#define		KEY_SLASH_PAD 		0x35 	; EXTRA KEY

#define		KEY_INS 	 		0x52 	; EXTRA KEY
#define		KEY_DEL 	 		0x53 	; EXTRA KEY

#define		KEY_RCTRL 			0x1D 	; EXTRA KEY
#define		KEY_ALTGR 	 		0x38 	; EXTRA KEY

#define		KEY_HOME 	 		0x47 	; EXTRA KEY
#define		KEY_REPAGE	 		0x49 	; EXTRA KEY
#define		KEY_AVPAGE	 		0x51 	; EXTRA KEY
#define		KEY_END 	 		0x4F 	; EXTRA KEY

#define		KEY_CALC 			0x21 	; EXTRA KEY
#define		KEY_MI_PC 	 		0x6B 	; EXTRA KEY

#define		KEY_GUI_LEFT 		0x5B 	; EXTRA KEY
#define		KEY_GUI_RIGHT 		0x5C 	; EXTRA KEY

#define		KEY_APPS 	 		0x5D 	; EXTRA KEY
#define		KEY_POWER 	 		0x5E 	; EXTRA KEY
#define		KEY_SLEEP 	 		0x5F 	; EXTRA KEY
#define		KEY_WAKE 	 		0x63 	; EXTRA KEY

#define		KEY_MM_PREV 		0x10 	; EXTRA KEY
#define		KEY_MM_NEXT 		0x19 	; EXTRA KEY
#define		KEY_MM_MUTE 		0x20 	; EXTRA KEY
#define		KEY_MM_PLAY 		0x22 	; EXTRA KEY
#define		KEY_MM_STOP 		0x24 	; EXTRA KEY
#define		KEY_MM_VOL_DOWN		0x2E 	; EXTRA KEY
#define		KEY_MM_VOL_UP 		0x30 	; EXTRA KEY
#define		KEY_MM_EMAIL 		0x6C 	; EXTRA KEY
#define		KEY_MM_MEDIA_SELECT	0x6D 	; EXTRA KEY

#define		KEY_WWW_HOME 		0x32 	; EXTRA KEY
#define		KEY_WWW_SEARCH 		0x65 	; EXTRA KEY
#define		KEY_WWW_FAV	 		0x66 	; EXTRA KEY
#define		KEY_WWW_REFRESH		0x67 	; EXTRA KEY
#define		KEY_WWW_STOP 		0x68 	; EXTRA KEY
#define		KEY_WWW_FORWARD		0x69 	; EXTRA KEY
#define		KEY_WWW_BACK 		0x6A 	; EXTRA KEY











/************************************************
 * 					DATA 	 					*
 ************************************************/
extern RingBuffer ring_buffer_keyboard __KEYBOARD__;










/************************************************
 * 					FUNCIONES 					*
 ************************************************/

#endif // _KEYBOARD_H_