/****************************************************
 * 					INCLUDES						*
 ****************************************************/
// kernel includes
#include <linux/init.h>		// __init, __exit
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>		// Lista de todas las funciones disponibles
#include <linux/cdev.h>
#include <linux/uaccess.h>	// Required for the copy to user function
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <asm/uaccess.h> 	// access_ok()

// project includes
#include "../inc/i2c_td3.h"
#include "i2c_td3_module.h"
#include "debug.h"
#include "GY_30.h"
#include "BBB_I2C.h"










/****************************************************
 * 					FORWARDS						*
 ****************************************************/
// Auxiliar functions
static int  write_opecode 	(uint opecode);
static void regs_init 		(void);

// char device functions
static 	int     i2c_td3_open  			(struct inode * inode,
										 struct file * 	file);

static 	ssize_t i2c_td3_read  			(struct file * 	file,
										 char *		    buffer,
										 size_t		    size,
										 loff_t *	    offset);

static 	ssize_t i2c_td3_write 			(struct file *	file,
										 const  char *	buffer,
										 size_t		    size,
										 loff_t *	    offset);
								
static 	ioctl_return_t 	i2c_td3_ioctl	(struct file *file,
            							 unsigned int cmd,
										 unsigned long arg);










/****************************************************
 *				AUXILIAR FUNCTIONS			    	*
 ****************************************************/
static int write_opecode(uint opecode)
{
	GY_30_t *		GY_30 = &i2c_td3.GY_30;
	i2c_con_t		config;
	i2c_irqenable_t	irqenable;



	DEBUG_LOG ("Writing opecode %d\n", opecode);



	if (i2c_td3.state != STATE_READY)
	{
		DEBUG_LOG ("Wating queue\n");
		while (wait_event_interruptible (i2c_td3.rw_wq.queue, 
										 atomic_dec_and_test(&i2c_td3.rw_wq.wake_up_cond)) != 0);
	}
	i2c_td3.state = STATE_SENDING;

	GY_30->tx_opecode = opecode;
	
	DEBUG_LOG ("Set write length\n");
	iowrite32(1, &GY_30->i2c->I2C_CNT);

	DEBUG_LOG ("Set device address\n");
	iowrite32(GY_30_ADDR_L, &GY_30->i2c->I2C_SA);

	DEBUG_LOG ("config as TX master\n");
	config.value 		= 0;
	config.bit.I2C_EN 	= 1;	// Module Enabled
	config.bit.MST 		= 1;	// Master Mode
	config.bit.TRX		= 1;	// Transmiter Mode
	config.bit.STT 		= 0;	// Start Bit
	config.bit.STP 		= 0;	// Stop Bit
	iowrite32(config.value, &GY_30->i2c->I2C_CON);  

	DEBUG_LOG ("Enable TX IRQ\n");
	irqenable.value			= 0;
	irqenable.bit.XRDY_IE	= 1;
	iowrite32(irqenable.value, &GY_30->i2c->I2C_IRQENABLE_SET);

	DEBUG_LOG ("Start TX\n");
	config.value		= 0;
	config.bit.I2C_EN 	= 1;	// Module Enabled
	config.bit.MST 		= 1;	// Master Mode
	config.bit.TRX		= 1;	// Transmiter Mode
	config.bit.STT 		= 1;	// Start Bit
	config.bit.STP 		= 0;	// Stop Bit
	iowrite32(config.value, &GY_30->i2c->I2C_CON); 



	// Aca cae la interrupcion de TX
	DEBUG_LOG ("Wating queue\n");
	while (wait_event_interruptible(i2c_td3.rw_wq.queue, 
									 atomic_dec_and_test(&i2c_td3.rw_wq.wake_up_cond)) != 0);

	GY_30->tx_opecode = 0;


	i2c_td3.state = STATE_READY;



	DEBUG_LOG ("opecode %d wrote\n", opecode);



	return 0;
}










static void regs_init (void)
{
	GY_30_t *		GY_30 = &i2c_td3.GY_30;
	i2c_con_t		config;



	DEBUG_LOG ("Initializing GY-30 registers\n");



	if (strcmp(GY_30->status, "okay") == 0)
	{
		DEBUG_LOG ("GY-30 status okay\n");



		DEBUG_LOG ("Disabling i2C\n");
		// Disable I2C
		config.value		= 0;
		config.bit.I2C_EN	= 0;
		iowrite32(0, &GY_30->i2c->I2C_CON);  	



		DEBUG_LOG ("setting i2C clock\n");
		// SCLK = 48MHz, ICLK = 12MHz
		// PSC = (SCLK / ICLK ) - 1 => PSC = 3
		iowrite8(3, &GY_30->i2c->I2C_PSC);

		if(GY_30->clock_freq == CLK_FREQ_100KHZ)
		{
			DEBUG_LOG ("clock freq = 100kHz\n");

			// freq_LOW = 200KHz
			// SCLL = (ICLK / freq_LOW) - 7 => SCLL = 53
			iowrite8(53, &GY_30->i2c->I2C_SCLL);  	

			// SCLH = (ICLK / freq_LOW) - 5 => SCLH = 55
			iowrite8(55, &GY_30->i2c->I2C_SCLH);
		}
		
		DEBUG_LOG ("Setting address\n");
		// Own (Master) Address
		iowrite8(I2C_OWN_ADDR, &GY_30->i2c->I2C_OA);

		// No wakeup, force idle, clock can be cut off
		iowrite32(0, &GY_30->i2c->I2C_SYSC);

		// Slave address
		iowrite32(GY_30_ADDR_L, &GY_30->i2c->I2C_SA);

		DEBUG_LOG ("Enabling I2C\n");
		// Habilita y configura como master
		config.value		= 0;
		config.bit.I2C_EN	= 1;
		config.bit.MST		= 1;
		config.bit.TRX		= 0;
		iowrite32(config.value, &GY_30->i2c->I2C_CON);
	}


	DEBUG_LOG ("GY-30 registers initialized\n");
}










/****************************************************
 * 				CHAR DEVICE FUNCTIONS				*
 ****************************************************/
static int i2c_td3_open (struct inode *inode, struct file *file)
{
	DEBUG_LOG ("Opening i2c_td3_dev\n");


	atomic_inc (&i2c_td3.num_open_devices);


	DEBUG_LOG ("Wating queue\n");
	while (wait_event_interruptible (i2c_td3.open_wq.queue, 
									 atomic_dec_and_test(&i2c_td3.open_wq.wake_up_cond)) != 0);

	regs_init();

	if( 0 != write_opecode(GY_30_POWER_ON) ) 
		return -EBUSY; // Device or resource busy

	if( 0 != write_opecode(GY_30_RESET) ) 
		return -EBUSY; // Device or resource busy

	if( 0 != write_opecode(GY_30_CONT_H_RES_MODE) ) 
		return -EBUSY; // Device or resource busy


	msleep(150); 


	DEBUG_INFO ("i2c_td3_dev opened\n");

	return 0;
}





static int i2c_td3_close (struct inode *inode, struct file *file)
{
	DEBUG_LOG ("Closing i2c_td3_dev\n");


	if (atomic_dec_and_test (&i2c_td3.num_open_devices))
	{
		write_opecode(GY_30_POWER_OFF);


		atomic_set (&i2c_td3.open_wq.wake_up_cond, 1);
		wake_up_interruptible (&i2c_td3.open_wq.queue);

		DEBUG_INFO ("i2c_td3_dev closed\n");
	}


	return 0;
}





static ssize_t i2c_td3_read (struct file *file, char *buffer, size_t size, loff_t *offset)
{
	GY_30_t *		GY_30 = &i2c_td3.GY_30;
	i2c_con_t		config;
	i2c_irqenable_t	irqenable;



	DEBUG_LOG ("Reading i2c_td3_dev\n");



	if (i2c_td3.state != STATE_READY)
	{
		DEBUG_LOG ("Wating queue\n");
		while (wait_event_interruptible (i2c_td3.rw_wq.queue,
										 atomic_dec_and_test(&i2c_td3.rw_wq.wake_up_cond)) != 0);
	}
	i2c_td3.state = STATE_RECEIVING1;



	DEBUG_LOG ("Setting msg size and address\n");
	iowrite32(2, &GY_30->i2c->I2C_CNT);
	iowrite32(GY_30_ADDR_L, &GY_30->i2c->I2C_SA);	// Slave Address



	DEBUG_LOG ("config as RX master\n");
	config.value		= 0;
	config.bit.I2C_EN 	= 1;	// Module Enabled
	config.bit.MST 		= 1;	// Master Mode
	config.bit.TRX 		= 0;	// Reciever Mode
	config.bit.STT 		= 0;	// Start Bit
	config.bit.STP 		= 0;	// Stop Bit
	iowrite32(config.value, &GY_30->i2c->I2C_CON);



	DEBUG_LOG ("Enable RX IRQ\n");
	irqenable.value			= 0;
	irqenable.bit.RRDY_IE 	= 1;
	iowrite32(irqenable.value, &GY_30->i2c->I2C_IRQENABLE_SET);



	DEBUG_LOG ("Start RX\n");
	config.value		= 0;
	config.bit.I2C_EN 	= 1;	// Module Enabled
	config.bit.MST 		= 1;	// Master Mode
	config.bit.TRX 		= 0;	// Reciever Mode
	config.bit.STT 		= 1;	// Start Bit
	config.bit.STP 		= 0;	// Stop Bit
	iowrite32(config.value, &GY_30->i2c->I2C_CON);



	// Aca cae la interrupcion de RX
	DEBUG_LOG ("Wating queue\n");
	while (wait_event_interruptible (i2c_td3.rw_wq.queue, 
									 atomic_dec_and_test(&i2c_td3.rw_wq.wake_up_cond)) != 0);



	DEBUG_LOG ("Stop RX\n");
	config.value		= 0;
	config.bit.I2C_EN 	= 1;	// Module Enabled
	config.bit.MST 		= 1;	// Master Mode
	config.bit.TRX 		= 0;	// Reciever Mode
	config.bit.STT 		= 0;	// Start Bit
	config.bit.STP 		= 1;	// Stop Bit
	iowrite32(config.value, &GY_30->i2c->I2C_CON);



	DEBUG_LOG ("i2c_td3_dev read end\n");
	DEBUG_INFO ("Read value: %d\n", GY_30->value);



	i2c_td3.state = STATE_READY;

	i2c_td3.num_reads++;

	// unsigned long copy_to_user(to, from, size);
	return copy_to_user(buffer, &GY_30->value, sizeof(GY_30->value));
}
 




static ssize_t i2c_td3_write (struct file *file, const char *buffer, size_t size, loff_t *offset)
{
   return -EPERM; // Operation not permitted
}





static ioctl_return_t i2c_td3_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	int * user_num_reads = (int *) arg;
	
	if (cmd == I2C_TD3_CMD_READ_NUM_READS)
	{
		return put_user(i2c_td3.num_reads, user_num_reads);
	}


	return -ENOTTY; // Inappropriate ioctl for device
}