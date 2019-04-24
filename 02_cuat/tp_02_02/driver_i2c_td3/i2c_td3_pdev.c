/****************************************************
 *					INCLUDES						*
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
static 	void pinmux (void *conf_module_pin,  pinmode_t 	pinmode,
					bool  pull_disable, 	bool 		pullup,
					bool  rx_active, 		bool  		slew_slow);

static 	bool clock_enable (void *cm_per_module_clkctrl);

static 	void unmap_regs (void);

// platform device functions
static	int		i2c_td3_probe	(struct platform_device *i2c_pdev);

static	int		i2c_td3_remove	(struct platform_device *i2c_pdev);

// platform device IRQ
static 	irqreturn_t i2c_td3_irq_handler	(int irq,
										 void *dev_id,
										 struct pt_regs *regs);










/****************************************************
 *				AUXILIAR FUNCTIONS					*
 ****************************************************/
static void pinmux (void *conf_module_pin,  pinmode_t 	pinmode,
					bool  pull_disable, 	bool 		pullup,
					bool  rx_active, 		bool  		slew_slow)
{
	uint32_t reg_value;



	reg_value =  ioread32 (conf_module_pin);

	reg_value &= ~(0x7F);
	reg_value |= (pinmode		<< 0)	|
				 (pull_disable	<< 3)	|
				 (pullup 		<< 4)	|
				 (rx_active 	<< 5)	|
				 (slew_slow 	<< 6);

	iowrite32 (reg_value, conf_module_pin);
}



static bool clock_enable (void *cm_per_module_clkctrl)
{
	int try = 0;
	const int max_tries = 5;

	CM_PER_I2C2_CLKCTRL_t read_value, write_value;



	write_value.value = ioread32 (cm_per_module_clkctrl);

	write_value.bit.module_mode = MODULE_MODE_ENABLE;

	iowrite32 (write_value.value, cm_per_module_clkctrl);

	do
	{
		if ( try > max_tries )
		{
			return -1;
		}

		read_value.value = ioread32 (cm_per_module_clkctrl);

		msleep (1);

		try++;
	} while (read_value.value != write_value.value);



	return 0;
}



static void unmap_regs (void)
{
	GY_30_t * GY_30 = &i2c_td3.GY_30;



	DEBUG_LOG ("Unmapping registers\n");



	if (GY_30->i2c_virq >= 0)
	{
		DEBUG_LOG ("Freeing GY_30->i2c_virq\n");
		free_irq(GY_30->i2c_virq, NULL);

		GY_30->i2c_virq = -1;
	}

	if (GY_30->i2c)
	{
		DEBUG_LOG ("Unmapping pdev->dev.of_node\n");
		iounmap (GY_30->i2c);

		GY_30->i2c = NULL;
	}

	if (GY_30->cm_per)
	{
		DEBUG_LOG ("Unmapping CM_PER\n");
		iounmap (GY_30->cm_per);

		GY_30->cm_per = NULL;
	}

	if (GY_30->control_module)
	{
		DEBUG_LOG ("Unmapping CONTROL_MODULE\n");
		iounmap (GY_30->control_module);

		GY_30->control_module = NULL;
	}
}










/****************************************************
 *				PLATFORM DEVICE FUNCTIONS			*
 ****************************************************/
static int i2c_td3_probe (struct platform_device *pdev)
{
	GY_30_t * GY_30 	= &i2c_td3.GY_30;
	mode_t i2c2_mode 	= PINMODE3;
	bool pull_disable	= false;
	bool pullup			= true;
	bool rx_active		= true;
	bool slew_slow		= false;



	DEBUG_LOG ("Probing platform device\n");





	DEBUG_LOG ("Mapping platform device registers\n");
	// Generar las entradas en las tablas de paginación de
	// los registros del platform device (ya cargados en memoria por el kernel)

	// CONTROL MODULE: para configurar el pinmux
	GY_30->control_module = ioremap (CONTROL_MODULE_ADDR, CONTROL_MODULE_LEN);
	if (GY_30->control_module == NULL)
	{
		DEBUG_ERROR ("Mapping CONTROL_MODULE\n");

		return -EADDRNOTAVAIL; // Cannot assign requested address
	}

	// CM_PER: para habilitar clock
	GY_30->cm_per = ioremap (CM_PER_ADDR, CM_PER_LEN);
	if (GY_30->cm_per == NULL) 
	{
		DEBUG_ERROR ("Mapping CM_PER\n");

		unmap_regs();

		return -EADDRNOTAVAIL; // Cannot assign requested address
	}

	// i2c of_node: para obtener 'status' y 'data' del I2C
	GY_30->i2c = of_iomap (pdev->dev.of_node, 0);
	if (GY_30->i2c == NULL) 
	{
		DEBUG_ERROR ("Mapping pdev->dev.of_node\n");

		unmap_regs();

		return -EADDRNOTAVAIL; // Cannot assign requested address
	}





	DEBUG_LOG ("Pin Muxing\n");

	pinmux (&GY_30->control_module->conf_uart1_ctsn,  i2c2_mode, 
			pull_disable, pullup, rx_active, slew_slow);

	pinmux (&GY_30->control_module->conf_uart1_rtsn,  i2c2_mode, 
			pull_disable, pullup, rx_active, slew_slow);





	DEBUG_LOG ("Reading Device Tree properties\n");
	
	if (of_property_read_u32 (pdev->dev.of_node, "clock-frequency", &GY_30->clock_freq))
	{
		DEBUG_ERROR ("Getting clock-frequency property\n");
		
		unmap_regs();

		return -EADDRNOTAVAIL; // Cannot assign requested address
	}
	DEBUG_INFO ("Clock frequency: %u Hz\n",GY_30->clock_freq);

	if (of_property_read_string (pdev->dev.of_node, "status", &GY_30->status))
	{
		DEBUG_ERROR ("Getting status property\n");
		
		unmap_regs();

		return -EADDRNOTAVAIL; // Cannot assign requested address
	}
	DEBUG_INFO ("Clock status: \"%s\"\n",GY_30->status);





	DEBUG_LOG ("Enabling clock\n");

	if (clock_enable(&GY_30->cm_per->CM_PER_I2C2_CLKCTRL))
	{
		DEBUG_ERROR ("Enabling clock\n");

		unmap_regs();

		return -EADDRNOTAVAIL; // Cannot assign requested address
	}





	DEBUG_LOG ("Getting IRQ\n");

	if ((GY_30->i2c_virq = platform_get_irq(pdev, 0)) < 0)
	{
		DEBUG_ERROR ("Getting virq\n");
		
		unmap_regs();

		return -EADDRNOTAVAIL; // Cannot assign requested address
	}
	




	DEBUG_LOG ("Installing IRQ handler\n");

	if (request_irq(GY_30->i2c_virq, (irq_handler_t) i2c_td3_irq_handler,
					IRQF_TRIGGER_RISING, DEVICE_NAME, NULL))
	{
			DEBUG_ERROR ("Installing IRQ\n");
		
			unmap_regs();

			return -EADDRNOTAVAIL; // Cannot assign requested address
	}



	DEBUG_LOG ("platform device probed\n");



	return 0;
}





static int i2c_td3_remove (struct platform_device *i2c_pdev)
{
	DEBUG_LOG ("removing platform device\n");
	
	unmap_regs();

	DEBUG_LOG ("platform device removed\n");

	return 0;
}










/****************************************************
 * 						IRQ HANDLER					*
 ****************************************************/
static irqreturn_t i2c_td3_irq_handler (int irq, void *dev_id, struct pt_regs *regs)
{
	GY_30_t *		GY_30 			= &i2c_td3.GY_30;
	i2c_irqstatus_t i2c_irq_status;
	i2c_irqenable_t aux_irqenable;



	DEBUG_LOG ("starting platform device IRQ\n");



	DEBUG_LOG ("get platform device IRQ status\n");
	i2c_irq_status.value = ioread32(&GY_30->i2c->I2C_IRQSTATUS);

	if(i2c_irq_status.bit.RRDY)
	{
		DEBUG_LOG ("In RX interrupt\n");

		

		DEBUG_LOG ("Read and concatenate data\n");
		if (i2c_td3.state == STATE_RECEIVING1)
		{
			DEBUG_LOG ("In STATE_RECEIVING1\n");

			GY_30->value = ((uint32_t) ioread8 (&GY_30->i2c->I2C_DATA)) << 8;

			DEBUG_LOG ("Cleaning interrupt flags\n");
			i2c_irq_status.bit.ARDY = 1;
			i2c_irq_status.bit.RRDY = 1;
			i2c_irq_status.bit.XRDY = 1;
			i2c_irq_status.bit.GC 	= 1;
			iowrite32 (i2c_irq_status.value, &GY_30->i2c->I2C_IRQSTATUS);	

			i2c_td3.state = STATE_RECEIVING2;
		}
		else if (i2c_td3.state == STATE_RECEIVING2)
		{
			DEBUG_LOG ("In STATE_RECEIVING2\n");

			GY_30->value |= ioread8 (&GY_30->i2c->I2C_DATA);

			DEBUG_LOG ("Disable RX IRQ\n");
			aux_irqenable.value 		= 0;
			aux_irqenable.bit.RRDY_IE	= 1;
			iowrite32 (aux_irqenable.value, &GY_30->i2c->I2C_IRQENABLE_CLR);

			DEBUG_LOG ("Cleaning interrupt flags\n");
			i2c_irq_status.bit.ARDY = 1;
			i2c_irq_status.bit.RRDY = 1;
			i2c_irq_status.bit.XRDY = 1;
			i2c_irq_status.bit.GC 	= 1;
			iowrite32 (i2c_irq_status.value, &GY_30->i2c->I2C_IRQSTATUS);		

			atomic_set (&i2c_td3.rw_wq.wake_up_cond, 1);
			wake_up_interruptible (&i2c_td3.rw_wq.queue);
		}
	}
	else if (i2c_irq_status.bit.XRDY)
	{
		DEBUG_LOG ("In TX interrupt\n");

		if (i2c_td3.state == STATE_SENDING)
		{
			DEBUG_LOG ("In STATE_SENDING\n");

			iowrite32 ((int)GY_30->tx_opecode, &GY_30->i2c->I2C_DATA);

			DEBUG_LOG ("Disable TX IRQ\n");
			aux_irqenable.value 		= 0;
			aux_irqenable.bit.XRDY_IE	= 1;
			iowrite32 (aux_irqenable.value, &GY_30->i2c->I2C_IRQENABLE_CLR);

			DEBUG_LOG ("Cleaning interrupt flags\n");
			i2c_irq_status.bit.ARDY = 1;
			i2c_irq_status.bit.RRDY = 1;
			i2c_irq_status.bit.XRDY = 1;
			i2c_irq_status.bit.GC 	= 1;
			iowrite32 (i2c_irq_status.value, &GY_30->i2c->I2C_IRQSTATUS);

			atomic_set (&i2c_td3.rw_wq.wake_up_cond, 1);
			wake_up_interruptible (&i2c_td3.rw_wq.queue);
		}
	}



	DEBUG_LOG ("platform device IRQ ended\n");



	return IRQ_HANDLED;
}