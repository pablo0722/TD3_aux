#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>



 
static int __init cind_init(void)
{
    // No pedir recursos aca. hacerlo en el open()
    printk(KERN_INFO "cind: controlador registrado");
    
    return 0;
}


static void __exit cind_exit(void)
{
    printk(KERN_INFO "cind: Controlador \"DESREGISTRADO\"");
}

module_init(cind_init);
module_exit(cind_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yo");
MODULE_DESCRIPTION("Controlador de fafafa");