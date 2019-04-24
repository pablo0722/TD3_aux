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

// project includes
#include "../inc/i2c_td3.h"
#include "i2c_td3_module.h"
#include "debug.h"
#include "GY_30.h"
#include "BBB_I2C.h"

// project sources
#include "i2c_td3_globals.c"
#include "i2c_td3_cdev.c"
#include "i2c_td3_pdev.c"




















/****************************************************
 * 				MODULE PROPERTIES					*
 ****************************************************/
// Para ver las propiedades del modulo: $modinfo
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("D_Angelo Pablo");
MODULE_DESCRIPTION("I2C Char Device Driver - TD3");
MODULE_VERSION("0.01");










/****************************************************
 * 					MODULE FUNCTIONS				*
 ****************************************************/
static int __init i2c_td3_init (void)
{
	// No pedir recursos aca. hacerlo en el open ()
	int status = 0;
	device_t *device = &i2c_td3.device;
	


	DEBUG_LOG ("Initializing module i2c_td3\n");


	
	// Allocates a range of char device numbers.
	// The major number will be chosen dynamically,
	//     and returned (along with the first minor number) in dev.
	// Returns zero or a negative error code.
	DEBUG_LOG ("allocating char device region\n");
	if ((status = alloc_chrdev_region (&device->dev_id, DEVICE_MINORBASE, DEVICE_MINORCOUNT, DEVICE_NAME)) < 0)
	{
		DEBUG_ERROR ("allocating char device region. Status: %d\n", status);

		return status;
	}
   


	DEBUG_LOG ("Creating class\n");
	if ((device->dev_class = class_create (THIS_MODULE, CLASS_NAME)) == NULL)
	{
		DEBUG_ERROR ("Class create\n");

		DEBUG_LOG ("unregistering char device\n");
		unregister_chrdev_region (device->dev_id, DEVICE_MINORCOUNT);

		return EFAULT;
	}
	


	// creates a device and registers it with sysfs
	DEBUG_LOG ("Creating device\n");
	if ((device_create (device->dev_class, DEVICE_PARENT_DEV, device->dev_id, DEVICE_DEV_DATA, DEVICE_NAME)) == NULL)
	{
		DEBUG_ERROR ("Device create\n");

		DEBUG_LOG ("Destroying class\n");
		class_destroy (device->dev_class);

		DEBUG_LOG ("unregistering char device\n");
		unregister_chrdev_region (device->dev_id, DEVICE_MINORCOUNT);
		
		return EFAULT;
	}
	


	DEBUG_LOG ("Initializing char device\n");
	cdev_init (&device->cdev.driver, &device->cdev.fops);
	
	DEBUG_LOG ("Adding char device\n");
	if ((status = cdev_add (&device->cdev.driver, device->dev_id, DEVICE_MINORCOUNT)) < 0 )
	{
		DEBUG_ERROR ("char device add\n");

		DEBUG_LOG ("Destroying device\n");
		device_destroy (device->dev_class, device->dev_id);

		DEBUG_LOG ("Destroying class\n");
		class_destroy (device->dev_class);

		DEBUG_LOG ("unregistering char device\n");
		unregister_chrdev_region (device->dev_id, DEVICE_MINORCOUNT);

		
		return status;
	}



	DEBUG_LOG ("Registering platform driver\n");
	// Al registrar el platform_driver se llama al probe() si encuentra el 'compatible' en el device tree
    if ( 0 > (status = platform_driver_register (&device->pdev.driver)) )
    {
		DEBUG_ERROR ("TD3Char: Error registering the platform driver\n");
        
        //Libero todo
		DEBUG_LOG ("uninitializing char device\n");
		cdev_del (&device->cdev.driver);

		DEBUG_LOG ("Destroying device\n");
		device_destroy (device->dev_class, device->dev_id);

		DEBUG_LOG ("Destroying class\n");
		class_destroy (device->dev_class);

		DEBUG_LOG ("unregistering char device\n");
		unregister_chrdev_region (device->dev_id, DEVICE_MINORCOUNT);

        return -EBUSY; // Device or resource busy
    }
	

	
	DEBUG_INFO ("Module i2c_td3 initialized <major = %d, minor = %d>\n",MAJOR (device->dev_id), MINOR (device->dev_id));


	
	return 0;
}





static void __exit i2c_td3_exit (void)
{
	device_t *device = &i2c_td3.device;



	DEBUG_LOG ("Exiting module i2c_td3\n");


	unmap_regs();
	
	DEBUG_LOG ("uninitializing char device\n");
	cdev_del (&device->cdev.driver);

	DEBUG_LOG ("Destroying device\n");
	device_destroy (device->dev_class, device->dev_id);

	DEBUG_LOG ("Destroying class\n");
	class_destroy (device->dev_class);

	DEBUG_LOG ("unregistering char device\n");
	unregister_chrdev_region (device->dev_id, DEVICE_MINORCOUNT);

	DEBUG_LOG ("unregistering platform driver\n");
	platform_driver_unregister (&device->pdev.driver);
	

	
	DEBUG_LOG ("Module i2c_td3 exited\n");
}





module_init (i2c_td3_init);
module_exit (i2c_td3_exit);