#include <linux/fs.h>
#include "scull.h"
#include "scull_helpers.h"


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

