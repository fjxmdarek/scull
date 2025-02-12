#include "scull.h"
#include <linux/module.h>


static void scull_setup_cdev(struct scull_dev *dev, int index)
{
 int err, devno = MKDEV(scull_major, scull_minor + index);
 cdev_init(&dev->cdev, &scull_fops);
 dev->cdev.owner = THIS_MODULE;
 dev->cdev.ops = &scull_fops;
 err = cdev_add (&dev->cdev, devno, 1);
 /* Fail gracefully if need be */
 if (err)
 printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

static int __init scull_init_module(void)
{
    int result, i;
    dev_t dev = 0;
    
    if (scull_major) {
        dev = MKDEV(scull_major, 0);
        result = register_chrdev_region(dev, scull_nr_devs, "scull");
    } else {
        result = alloc_chrdev_region(&dev, 0, scull_nr_devs, "scull");
        scull_major = MAJOR(dev);
    }
    if (result < 0) {
        printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
        return result;
    }
    
    /* 初始化并注册各个设备 */
    for (i = 0; i < scull_nr_devs; i++) {
        scull_devices[i].quantum = scull_quantum;
        scull_devices[i].qset = scull_qset;
        sema_init(&scull_devices[i].sem, 1);
        scull_setup_cdev(&scull_devices[i], i);
    }
    
    printk(KERN_INFO "scull: registered with major number %d\n", scull_major);
    return 0;
}

static void __exit scull_cleanup_module(void)
{
    int i;
    for (i = 0; i < scull_nr_devs; i++) {
        cdev_del(&scull_devices[i].cdev);
    }
    unregister_chrdev_region(MKDEV(scull_major, 0), scull_nr_devs);
    printk(KERN_INFO "scull: module unloaded\n");
}


module_init(scull_init_module);
module_exit(scull_cleanup_module);