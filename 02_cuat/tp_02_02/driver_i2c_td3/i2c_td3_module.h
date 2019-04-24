#ifndef MODULE_I2C_TD3_MODULE_H_
#define MODULE_I2C_TD3_MODULE_H_










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

// project includes
#include "BBB_I2C.h"










/****************************************************
 * 				DEVICE DEFINES						*
 ****************************************************/
// cat /proc/devices
// cat /dev/
#define DEVICE_MINORCOUNT	1
#define DEVICE_MINORBASE	0
#define CLASS_NAME			"i2c_td3_class"
#define DEVICE_NAME			"i2c_td3_dev"
#define DEVICE_PARENT_DEV	NULL
#define DEVICE_DEV_DATA		NULL

// IOCTL defines
#ifdef HAVE_UNLOCKED_IOCTL
	typedef long ioctl_return_t;
#else
	typedef int ioctl_return_t;
#endif










/****************************************************
 * 					PROTOTYPES						*
 ****************************************************/
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

static 	int     i2c_td3_close			(struct inode * inode,
										 struct file * 	file);
								
static 	ioctl_return_t 	i2c_td3_ioctl	(struct file *file,
            							 unsigned int cmd,
										 unsigned long arg);

// platform device functions
static 	int		i2c_td3_probe	(struct platform_device *i2c_pdev);

static 	int		i2c_td3_remove	(struct platform_device *i2c_pdev);










/****************************************************
 * 					STRUCTS							*
 ****************************************************/
// Spinlock: es un bloqueo (lock) que hace que un thread que intenta adquirirlo (el lock)
//			simplemente espere en un bucle (spin) mientras comprueba repetidamente
//			si el bloqueo (lock) está disponible.





// Wait Queues: ...
typedef struct
{
	wait_queue_head_t	queue;
	atomic_t			wake_up_cond;
} wait_queue_t;





typedef struct
{
			struct	platform_driver	driver;
} pdev_t;





typedef struct
{
	struct	cdev			driver;
	struct	file_operations	fops;
} cdev_t;





typedef struct
{
			dev_t	dev_id;
	struct	class *	dev_class;
			cdev_t	cdev;
			pdev_t	pdev;
} device_t;





typedef enum
{
	STATE_READY,
	STATE_SENDING,
	STATE_RECEIVING1,
	STATE_RECEIVING2,
} state_machine_t;





typedef struct
{
	control_module_offset_t *	control_module;
	cm_per_offset_t *			cm_per;
	i2c_reg_t *					i2c;
	char *						rx_buff;
	uint						rx_buff_pos;
	char *						tx_buff;
	uint						tx_buff_pos;
	uint						tx_buff_size;
	int							i2c_virq;
	unsigned char				tx_opecode;
	uint						clock_freq;
	const char *				status;
	uint						value;	// Valor leido del sensor
} GY_30_t;





typedef struct
{
	wait_queue_t	rw_wq;
	wait_queue_t	open_wq;
	atomic_t		num_open_devices;
	device_t		device;
	GY_30_t			GY_30;
	state_machine_t	state;
	int 			num_reads;
} i2c_td3_t;










/****************************************************
 * 			    GLOBAL VARIABLES					*
 ****************************************************/
extern i2c_td3_t i2c_td3;










#endif // MODULE_I2C_TD3_MODULE_H_