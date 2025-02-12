#ifndef SCULL_H
#define SCULL_H

#include <linux/module.h>         // 模块相关操作
#include <linux/kernel.h>         // printk函数等
#include <linux/init.h>           // 宏定义
#include <linux/fs.h>            // 文件系统相关操作
#include <linux/miscdevice.h>    // misc设备相关操作
#include <linux/uaccess.h>       // 用户空间和内核空间数据交换
#include <linux/uio.h> 
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/string.h>


#include "scull_helpers.h"

MODULE_LICENSE("GPL"); 

#define SCULL_MAJOR 0
#define SCULL_QUANTUM 0
#define SCULL_NR_DEVS 4

// 在 scull_fops 初始化之前添加前向声明
extern ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
extern ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
extern int scull_open(struct inode *inode, struct file *filp);
extern int scull_release(struct inode *inode, struct file *filp);
extern loff_t scull_llseek(struct file *filp, loff_t off, int whence);
//dev_t i_rdev;
//struct cdev *i_cdev;
struct scull_dev { 
    struct scull_qset *data; /* Pointer to first quantum set */ 
    int quantum; /* the current quantum size */ 
    int qset; /* the current array size */ 
    unsigned long size; /* amount of data stored here */ 
    unsigned int access_key; /* used by sculluid and scullpriv */ 
    struct semaphore sem; /* mutual exclusion semaphore */ 
    struct cdev cdev; /* Char device structure */
   };

struct scull_qset {
    void **data;
    struct scull_qset *next; 
   }; 

extern struct file_operations scull_fops;
extern int scull_major;
extern int scull_minor;
extern int scull_quantum;
extern int scull_qset;
extern int scull_nr_devs;
extern struct scull_dev scull_devices[SCULL_NR_DEVS];
#endif /* SCULL_H */


