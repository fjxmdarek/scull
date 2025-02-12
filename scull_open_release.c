#include <linux/fs.h>
#include "scull.h"

int scull_trim(struct scull_dev *dev)
{
 struct scull_qset *next, *dptr;
 int qset = dev->qset; /* "dev" is not-null */
 int i;
 for (dptr = dev->data; dptr; dptr = next)
 { /* all the list items */
 if (dptr->data) {
 for (i = 0; i < qset; i++)
 kfree(dptr->data[i]);
 kfree(dptr->data);
 dptr->data = NULL;
 }
 next = dptr->next;
 kfree(dptr);
 }
 dev->size = 0;
 dev->quantum = scull_quantum;
 dev->qset = scull_qset;
 dev->data = NULL;
 return 0;
}

int scull_open(struct inode *inode, struct file *filp)
{
 printk(KERN_INFO "scull_open: success\n");
 struct scull_dev *dev; /* device information */
 dev = container_of(inode->i_cdev, struct scull_dev, cdev);
 filp->private_data = dev; /* for other methods */
 /* now trim to 0 the length of the device if open was write-only */
 if ( (filp->f_flags & O_ACCMODE) == O_WRONLY)
 {
 scull_trim(dev); /* ignore errors */
 }
 return 0; /* success */
}


int scull_release(struct inode *inode, struct file *filp)
{
 return 0;
}


loff_t scull_llseek(struct file *filp, loff_t off, int whence)
{
struct scull_dev *dev = filp->private_data;
loff_t newpos;
switch(whence)
{
case 0: /* SEEK_SET */
newpos = off;
break;
case 1: /* SEEK_CUR */
newpos = filp->f_pos + off;
break;
case 2: /* SEEK_END */
newpos = dev->size + off;
break;
default: /* can't happen */
return -EINVAL;
}
if (newpos < 0)
return -EINVAL;
filp->f_pos = newpos;
return newpos;
}
