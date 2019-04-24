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










/****************************************************
 * 				LOCAL VARIABLES					    *
 ****************************************************/
static const struct of_device_id i2c_of_match[] =
{
    { .compatible = "td3,omap4-i2c" },
    {},
};










/****************************************************
 * 				GLOBAL VARIABLES					*
 ****************************************************/
i2c_td3_t i2c_td3 =
{
    // Wait Queues
    .rw_wq =
    {
        .queue 			= __WAIT_QUEUE_HEAD_INITIALIZER(i2c_td3.rw_wq.queue),
        .wake_up_cond	= ATOMIC_INIT (1),
    },

    .open_wq =
    {
        .queue 			= __WAIT_QUEUE_HEAD_INITIALIZER(i2c_td3.open_wq.queue),
        .wake_up_cond	= ATOMIC_INIT (1),
    },





    // identificadores de dispositivo
    .device =
    {
        .cdev 	=
        {
            .fops       = 
            {
                .owner 		        = THIS_MODULE,
                .open 	            = i2c_td3_open,
                .read 		        = i2c_td3_read,
                .write 		        = i2c_td3_write,
                .release 	        = i2c_td3_close,

            #ifdef HAVE_UNLOCKED_IOCTL
                .unlocked_ioctl 	= i2c_td3_ioctl, // The BKL (Big Kernel Lock) is not taken prior to the call. Code should be written with its own locking
            #else
                .ioctl 	            = i2c_td3_ioctl,
            #endif
            #ifdef HAVE_COMPAT_IOCTL
                //.compact_ioctl 	    = i2c_td3_compact_ioctl, // this method will be called whenever a 32-bit process calls ioctl() on a 64-bit system
            #endif
            },
        },
        .pdev	=
        {
            .driver 	=
            {
                .probe  = i2c_td3_probe,
                .remove = i2c_td3_remove,
                .driver =
                {
                    .name 			= DEVICE_NAME,
                    .of_match_table = of_match_ptr (i2c_of_match),
                },
            },
        },
    },





    // GY-30 info
    .GY_30 = 
    {
        .control_module = NULL,
        .cm_per         = NULL,
        .i2c            = NULL,
        .rx_buff        = NULL,
        .rx_buff_pos    = 0,
        .tx_buff        = NULL,
        .tx_buff_pos    = 0,
        .tx_buff_size   = 0,
        .i2c_virq       = -1,
        .tx_opecode     = 0,
        .clock_freq	    = 0,
        .status         = NULL,
        .value          = 0,
    },





    .state              = STATE_READY,
    .num_reads          = 0,
    .num_open_devices   = ATOMIC_INIT (0)
};



MODULE_DEVICE_TABLE (of, i2c_of_match);