#ifndef MODULE_GY_30_H_
#define MODULE_GY_30_H_










// REGISTER ADDRESS



// Clock-frequency from device tree
#define CLK_FREQ_100KHZ	100000



// Own master address
#define 	I2C_OWN_ADDR 		0x36



// Device address when address pin LOW
#define GY_30_ADDR_L	0b00100011

// Device address when address pin HIGH
#define GY_30_ADDR_H	0x01011100





// OPECODES

// No active state.
#define GY_30_POWER_OFF			0b00000000

// Waiting for measurement command.
#define GY_30_POWER_ON			0b00000001

// Reset Data register value.
// Reset command is not acceptable in Power Down mode.
#define GY_30_RESET				0b00000111

// High Resolution Mode
// Start measurement at 1lx resolution.
// Measurement Time is typically 120ms.
#define GY_30_CONT_H_RES_MODE	0b00010000

// High Resolution Mode
// Start measurement at 0.5lx resolution.
// Measurement Time is typically 120ms.
#define GY_30_CONT_H_RES_MODE2	0b00010001

// Low Resolution Mode
// Start measurement at 4lx resolution.
// Measurement Time is typically 16ms.
#define GY_30_CONT_L_RES_MODE	0b00010011










#endif // MODULE_GY_30_H_