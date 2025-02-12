#include "scull.h"
int scull_major = SCULL_MAJOR;
int scull_minor ;

int scull_quantum = SCULL_QUANTUM;
int scull_qset ;
int scull_nr_devs = 4;  // 根据你的需求设置设备数量
struct scull_dev scull_devices[4]; // 定义一个设备数组，大小与 scull_nr_devs 相同

struct file_operations scull_fops = {
    .owner = THIS_MODULE, 
    .llseek = scull_llseek, 
    .read = scull_read, 
    .write = scull_write,  
    .open = scull_open, 
    .release = scull_release, 
};

MODULE_LICENSE("GPL");