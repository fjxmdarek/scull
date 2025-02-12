#include "scull.h"
#include <linux/uaccess.h>


struct scull_qset *scull_follow(struct scull_dev *dev, int n)
{
    struct scull_qset *qs = dev->data;

    if (!qs) {
        qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
        if (!qs)
            return NULL;
        memset(qs, 0, sizeof(struct scull_qset));
    }
    while (n--) {
        if (!qs->next) {
            qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
            if (!qs->next)
                return NULL;
            memset(qs->next, 0, sizeof(struct scull_qset));
        }
        qs = qs->next;
    }
    return qs;
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
 struct scull_dev *dev = filp->private_data;
 struct scull_qset *dptr; /* the first listitem */
 int quantum = dev->quantum, qset = dev->qset;
 int itemsize = quantum * qset; /* how many bytes in the listitem */
 int item, s_pos, q_pos, rest;
 ssize_t retval = 0;
 if (down_interruptible(&dev->sem))
 return -ERESTARTSYS;
 if (*f_pos >= dev->size)
 goto out;
 if (*f_pos + count > dev->size)
 count = dev->size - *f_pos;
 /* find listitem, qset index, and offset in the quantum */
 item = (long)*f_pos / itemsize;
 rest = (long)*f_pos % itemsize;
 s_pos = rest / quantum;
 q_pos = rest % quantum;
 /* follow the list up to the right position (defined elsewhere) */
 dptr = scull_follow(dev, item);
 if (dptr == NULL || !dptr->data || ! dptr->data[s_pos])
 goto out; /* don't fill holes */
 /* read only up to the end of this quantum */
 if (count > quantum - q_pos)
 count = quantum - q_pos;
 if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count))
 {
 retval = -EFAULT;
 goto out;
 }
 *f_pos += count;
 retval = count;
out:
 up(&dev->sem);
 return retval;
}


ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
 struct scull_dev *dev = filp->private_data;
 struct scull_qset *dptr;
 int quantum = dev->quantum, qset = dev->qset;
 int itemsize = quantum * qset;
 int item, s_pos, q_pos, rest;
 ssize_t retval = -ENOMEM; /* value used in "goto out" statements */
 if (down_interruptible(&dev->sem))
 return -ERESTARTSYS;
 /* find listitem, qset index and offset in the quantum */
 item = (long)*f_pos / itemsize;
 rest = (long)*f_pos % itemsize;
 s_pos = rest / quantum;
 q_pos = rest % quantum;
 /* follow the list up to the right position */
 dptr = scull_follow(dev, item);
 if (dptr == NULL)
 goto out;
 if (!dptr->data)
 {
 dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
 if (!dptr->data)
 goto out;
 memset(dptr->data, 0, qset * sizeof(char *));
 }
 if (!dptr->data[s_pos])
 {
 dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
 if (!dptr->data[s_pos])
 goto out;
 }
 /* write only up to the end of this quantum */
 if (count > quantum - q_pos)
 count = quantum - q_pos;
 if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count))
 {
 retval = -EFAULT;
 goto out;
 }
 *f_pos += count;
 retval = count;
 /* update the size */
 if (dev->size < *f_pos)
 dev->size = *f_pos;
out:
 up(&dev->sem);
 return retval;
}
