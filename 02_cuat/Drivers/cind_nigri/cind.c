#include <asm-generic/errno.h>
#include <asm-generic/errno-base.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/version.h>

#define CIND_CLASS         "chardrv"
#define CIND_MINORBASE     0
#define CIND_MINORCOUNT    1
#define CIND_NAME          "ChristiaN_Controlador"
#define CIND_NAME_SHORT    "chn_trler"

#define CIND_DEV_PARENT    NULL
#define CIND_DEV_DATA      NULL

/*Variables globales en espacio de kernel, conviene identificarlas claramente*/
static dev_t cind_dev;
static struct class *cind_dev_class;
static struct cdev cind_dev_char;

int cind_open(struct inode *inode, struct file *filp)
{
   printk(KERN_INFO "-->ChristiaN: Controlador abierto \n");
   return 0;
}

int cind_close(struct inode *inode, struct file *filp)
{
   printk(KERN_INFO "-->ChristiaN: Controlador cerrado \n");
   return 0;
}

struct file_operations cind_dev_fops = {
   .owner = THIS_MODULE,
   .open = cind_open,
   .release = cind_close
};

static int __init cind_init(void)
{
   int status=0;
   
   printk(KERN_INFO "-->ChristiaN: Controlador registradandose");

   if((status=alloc_chrdev_region(&cind_dev, CIND_MINORBASE, CIND_MINORCOUNT, CIND_NAME)) < 0)
   {
      printk(KERN_ERR "-->ChristiaN: %d (%s %d)<----\n", status, __FUNCTION__, __LINE__);
      return(status);
   }
   
   if((cind_dev_class = class_create(THIS_MODULE, CIND_CLASS)) == NULL)
   {
      unregister_chrdev_region(cind_dev, CIND_MINORCOUNT);
      return EFAULT;
   }

   if((device_create(cind_dev_class, CIND_DEV_PARENT, cind_dev, CIND_DEV_DATA, CIND_NAME_SHORT)) == NULL)
   {
      class_destroy(cind_dev_class);
      unregister_chrdev_region(cind_dev, CIND_MINORCOUNT);
      return EFAULT ;
   }

   cdev_init(&cind_dev_char, &cind_dev_fops);
   if((status=cdev_add(&cind_dev_char, cind_dev, CIND_MINORCOUNT)) < 0)
   {
      device_destroy(cind_dev_class, cind_dev);
      class_destroy(cind_dev_class);
      unregister_chrdev_region(cind_dev, CIND_MINORCOUNT);
      return(status);
   }
   
   printk(KERN_INFO "-->ChristiaN: Controlador registrado con <Major, Minor>: <%d, %d>\n", MAJOR(cind_dev), MINOR(cind_dev));
   
   return(status);
}
 
static void __exit cind_exit(void)
{
   
   printk(KERN_INFO "-->ChristiaN: Controlador desregistrandose");
   
   cdev_del(&cind_dev_char);
   device_destroy(cind_dev_class, cind_dev);
   class_destroy(cind_dev_class);
   unregister_chrdev_region(cind_dev, CIND_MINORCOUNT);
   
   printk(KERN_INFO "-->ChristiaN: Controlador desregistrado");
}

module_init(cind_init);
module_exit(cind_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChristiaN");
MODULE_DESCRIPTION("Controlador Minimo");
