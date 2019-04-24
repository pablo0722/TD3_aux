#ifndef MODULE_BBB_I2C_H_
#define MODULE_BBB_I2C_H_

// a master device is the device which initiates a data transfer on the bus
// and generates the clock signals to permit that transfer.

// Registros mapeados a memoria
// Informacion obtenida de "AM335x Sitara_TM Processors - Technical Reference Manual"
// La informacion referida a un pin en particular (PinMux) fue obtenida del "AM3358 Datasheet":
// 		I2C2_SCL (pin 19): pag 44
// 		I2C2_SDA (pin 20): pag 45










/****************************************************
 * 					INCLUDES						*
 ****************************************************/
// kernel includes
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>		// Lista de todas las funciones disponibles
#include <linux/init.h>		// __init, __exit
#include <linux/module.h>
#include <linux/list.h>










/****************************************************
 * 				COMMON DEFINES						*
 ****************************************************/
#ifndef __RO
	#define __RO
#endif // __RO

#ifndef __WO
	#define __WO
#endif // __WO

#ifndef __RW
	#define __RW
#endif // __RW










/****************************************************
 * 					TYPEDEFS						*
 ****************************************************/
typedef unsigned int 		uint32_t;










/****************************************************
 * 				TYPEDEFS (OFFSETS)					*
 ****************************************************/
// Control Module Registers (PinMux) offsets - pag. 1368
typedef struct
{
	__RO uint32_t others[0x25E];
	
	__RO uint32_t conf_uart1_ctsn;	// offset 0x978 - I2C2_SDA: Mode = 3 (pag. 45 del datasheet)
	__RO uint32_t conf_uart1_rtsn;	// offset 0x97C - I2C2_SCL: Mode = 3 (pag. 44 del datasheet)
} control_module_offset_t;



// CM_PER Registers (ClkEnable) offsets - pag. 1361
typedef struct
{
	__RO uint32_t others[0x11];
	
	__RO uint32_t CM_PER_I2C2_CLKCTRL;	// offset 0x44
} cm_per_offset_t;










/****************************************************
 * 				TYPEDEFS (REGISTERS)				*
 ****************************************************/
// Clock Control Register - pag. 1178
typedef enum
{
	MODULE_MODE_DISABLE,
	MODULE_MODE_rESERVED1,
	MODULE_MODE_ENABLE,
	MODULE_MODE_rESERVED2
} module_mode_t;

typedef enum
{
	IDLE_ST_FUNCTIONAL,	// Module is fully functional, including OCP
	IDLE_ST_TRANSITION, // Module is performing transition: wakeup,
						// sleep, or sleep abortion
	IDLE_ST_IDLE,		// Module is in Idle mode (only OCP part). It is functional if
						// using separate functional clock
	IDLE_ST_DISABLED	// Module is disabled and cannot be accessed
} idle_st_t;

typedef union
{
	uint32_t value;
	struct
	{
		__RW module_mode_t	module_mode : 2;
		__RO uint32_t 		reserved1 	: 14;
		__RO uint32_t 		idle_st 	: 2;
		__RO uint32_t 		reserved2 	: 14;
	} bit;
} CM_PER_I2C2_CLKCTRL_t;



// PinMux mode - Ver AM3358 Datasheet (pag. 44/45 para I2C2_SCL/I2C2_SDA)
typedef enum
{
	PINMODE0,
	PINMODE1,
	PINMODE2,
	PINMODE3
} pinmode_t;



// I2C IRQSTATUS Register - pag. 4501
typedef union
{
	uint32_t value;
	struct
	{
		__RW uint32_t AL		:1;	// Arbitration lost: 								0 = Normal operation, 			1 = Arbitration lost detected
		__RW uint32_t NACK		:1;	// No acknowledgment 		(Write '1' to clear): 	0 = Normal operation, 			1 = Not Acknowledge detected
		__RW uint32_t ARDY		:1;	// Register access ready 	(Write '1' to clear):  	0 = Module busy, 				1 = Access ready
		__RW uint32_t RRDY		:1; // Receive data ready 		(Write '1' to clear):	0 = No data available, 			1 = Receive data available
		__RW uint32_t XRDY		:1; // Transmit data ready 		(Write '1' to clear):	0 = Transmission ongoing,		1 = Transmit data ready
		__RW uint32_t GC		:1; // General call				(Write '1' to clear):	0 = No general call detected,	1 = General call address detected
		__RW uint32_t STC		:1; // Start Condition:									0 = No action, 					1 = Start condition detected
		__RW uint32_t AERR		:1; // Access Error:									0 = No action, 					1 = Access error
		__RW uint32_t BF		:1; // Bus Free:										0 = No action, 					1 = Bus free
		__RW uint32_t AAS		:1; // Address recognized as slave:						0 = No action, 					1 = Address recognized
		__RW uint32_t XUDF		:1; // Transmit underflow:								0 = Normal operation, 			1 = Transmit underflow
		__RW uint32_t ROVR		:1;	// Receive overrun:									0 = Normal operation,			1 = Receiver overrun
		__RW uint32_t BB		:1; // Bus busy:										0 = Bus is free,				1 = Bus is occupied
		__RW uint32_t RDR		:1; // Receive draining:								0 = Receive draining inactive,	1 = Receive draining enabled
		__RW uint32_t XDR		:1; // Transmit draining:								0 = Transmit draining inactive, 1 = Transmit draining enabled
		__RO uint32_t reserved	:17;
	} bit;
} i2c_irqstatus_t;

// I2C IRQENABLE Register - pag. 4505
typedef union
{
	uint32_t value;
	struct
	{
		uint32_t AL_IE		:1;
		uint32_t NACK_IE	:1;
		uint32_t ARDY_IE	:1;	// Register access ready interrupt enable. 	value: 0 = disable, 1 = enable.
		uint32_t RRDY_IE	:1;	// Receive data ready interrupt enable. 	value: 0 = disable, 1 = enable. In I2C_IRQSTATUS bit is set when receive data is available
		uint32_t XRDY_IE	:1;	// Transmit data ready interrupt enable. 	value: 0 = disable, 1 = enable. In I2C_IRQSTATUS bit is set when transmit data is available
		uint32_t GC_IE		:1;
		uint32_t STC_IE		:1;
		uint32_t AERR_IE	:1;
		uint32_t BF_IE		:1;
		uint32_t AAS_IE		:1;
		uint32_t XUDF		:1;
		uint32_t ROVR		:1;
		uint32_t reserved1	:1;
		uint32_t RDR_IE		:1;
		uint32_t XDR_IE		:1;
		uint32_t reserved2	:17;
	} bit;
} i2c_irqenable_t;

// I2C_CON Register - pag. 4523
typedef union
{
	uint32_t value;
	struct
	{
		uint32_t STT		:1;	// Start condition: 			0 = No start condition detected,	1 = Start condition queried
		uint32_t STP		:1;	// Stop condition: 				0 = No stop condition detected,		1 = Stop condition queried
		uint32_t reserved1	:2;
		uint32_t XOA3		:1;
		uint32_t XOA2		:1;
		uint32_t XOA1		:1;
		uint32_t XOA0		:1;
		uint32_t XSA		:1;
		uint32_t TRX		:1; // Transmitter/receiver mode:	0 = Receiver mode,					1 = Transmitter mode
		uint32_t MST		:1; // Master/slave mode: 			0 = Slave mode,						1 = Master mode
		uint32_t STB		:1;
		uint32_t OPMODE		:2;
		uint32_t reserved2	:1;
		uint32_t I2C_EN		:1; // I2C module enable: 			0 = Controller in reset,			1 = Module enabled
		uint32_t reserved3	:16;
	} bit;
} i2c_con_t;

// I2C SYSC Register - pag. 4493
typedef union
{
	uint32_t value;
	struct
	{
		uint32_t AUTOIDLE		:1;	// Autoidle:					0 = Autoidle disabled,	1 = Autoidle enabled
		uint32_t SRST			:1; // SoftReset:					0 = Normal mode,		1 = The module is reset
		uint32_t ENAWAKEUP		:1;	// Enable Wakeup control:		0 = Wakeup disabled,	1 = Wakeup enabled
		uint32_t IDLEMODE		:2;	// Idle Mode selection:			0 = Idle forced,		1 = No Idle mode,		2 = Smart Idle mode, 3 = Smart-idle wakeup. Available only on I2C0.
		uint32_t reserved1		:3;
		uint32_t CLKACTIVITY	:2;	// Clock Activity selection:	0 = Both clocks can be cut off, 1 = Only Interface/OCP clock must be kept active; system clock can be cut off, 2 = Only system clock must be kept active; Interface/OCP clock can be cut off, 3 = Both clocks must be kept active
		uint32_t reserved2		:22;
	} bit;
} i2c_sysc_t;

// I2C Registers offset - pag. 4490
typedef struct
{
	uint32_t		I2C_REVNB_LO;			// offset: 0x00 - Module Revision Register (low bytes)
	uint32_t		I2C_REVNB_HI;			// offset: 0x04 - Module Revision Register (high bytes)
	uint32_t		reserved1[2];
	i2c_sysc_t		I2C_SYSC;				// offset: 0x10 - System Configuration Register
	uint32_t		reserved2[4];
	i2c_irqstatus_t	I2C_IRQSTATUS_RAW;		// offset: 0x24 - I2C Status Raw Register
	i2c_irqstatus_t	I2C_IRQSTATUS;			// offset: 0x28 - I2C Status Register
	i2c_irqenable_t	I2C_IRQENABLE_SET;		// offset: 0x2C - I2C Interrupt Enable Set Register
	i2c_irqenable_t	I2C_IRQENABLE_CLR;		// offset: 0x30 - I2C Interrupt Enable Clear Register
	uint32_t		I2C_WE;					// offset: 0x34 - I2C Wakeup Enable Register
	uint32_t		I2C_DMARXENABLE_SET;	// offset: 0x38 - Receive DMA Enable Set Register
	uint32_t		I2C_DMATXENABLE_SET;	// offset: 0x3C - Transmit DMA Enable Set Register
	uint32_t		I2C_DMARXENABLE_CLR;	// offset: 0x40 - Receive DMA Enable Clear Register
	uint32_t		I2C_DMATXENABLE_CLR;	// offset: 0x44 - Transmit DMA Enable Clear Register
	uint32_t		I2C_DMARXWAKE_EN;		// offset: 0x48 - Receive DMA Wakeup Register
	uint32_t		reserved3[17];
	uint32_t		I2C_SYSS;				// offset: 0x90 - System Status Register
	uint32_t		I2C_BUF;				// offset: 0x94 - Buffer Configuration Register
	uint32_t		I2C_CNT;				// offset: 0x98 - Data Counter Register
	uint32_t		I2C_DATA;				// offset: 0x9C - Data Access Register
	uint32_t		reserved4[1];
	i2c_con_t		I2C_CON;				// offset: 0xA4 - I2C Configuration Register
	uint32_t		I2C_OA;					// offset: 0xA8 - I2C Own Address Register
	uint32_t		I2C_SA;					// offset: 0xAC - I2C Slave Address Register
	uint32_t		I2C_PSC;				// offset: 0xB0 - I2C Clock Prescaler Register
	uint32_t		I2C_SCLL;				// offset: 0xB4 - I2C SCL Low Time Register
	uint32_t		I2C_SCLH;				// offset: 0xB8 - I2C SCL High Time Register
	uint32_t		I2C_SYSTEST;			// offset: 0xBC - System Test Register
	uint32_t		I2C_BUFSTAT;			// offset: 0xC0 - I2C Buffer Status Register
	uint32_t		I2C_OA1;				// offset: 0xC4 - I2C Own Address 1 Register
	uint32_t		I2C_OA2;				// offset: 0xC8 - I2C Own Address 2 Register
	uint32_t		I2C_OA3;				// offset: 0xCC - I2C Own Address 3 Register
	uint32_t		I2C_ACTOA;				// offset: 0xD0 - Active Own Address Register
	uint32_t		I2C_SBLOCK;				// offset: 0xD4 - I2C Clock Blocking Enable Register
} i2c_reg_t;











/****************************************************
 * 					DEFINES							*
 ****************************************************/
// Control Module Registers (PinMux) - pag. 180
#define CONTROL_MODULE_ADDR				0x44E10000
#define CONTROL_MODULE_LEN				0x2000



// Clock Module Peripheral Registers (ClkEnable) - pag. 179
#define CM_PER_ADDR						0x44E00000
#define CM_PER_LEN						0x4000










#endif // MODULE_BBB_I2C_H_