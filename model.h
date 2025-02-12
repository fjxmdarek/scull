#include <linux/module.h>         
#ifndef __MODEL_H__
#define __MODEL_H__

#define CONFIG_SOFTWARE_VERSION          "1.0.0.0"
#define CONFIG_HARDWARE_VERSION          "SD2351”
#define __DATE__                        "2025-2-12"
#define __TIME__                        "10:00:00"
printk("Keypad-"CONFIG_SOFTWARE_VERSION",Build at "__DATE__","__TIME__".\n");

/*
1.0.0.0 -> 1.0.0.0 :  This is a character driver written in reference to textbooks 
                      in order to learn kernel development
*/
/*********************************************************************
修改时间：2025-2-12
发布者 ： zxh
版本号 ：1.0.0.0
更改原因：
更改措施：
编译条件：CONFIG_RESVERVE_FBMEN
**********************************************************************/
#define CONFIG_RESVERVE_FBMEN 1
//#define CONFIG_RESVERVE_FBMEM_OFFSET (CONFIG_DDR_SIZE-CONFIG_RESVERVE_FBMEM_SIZE)
//#define CONFIG_RESVERVE_FBMEM_SIZE (512*1024)
//#define CONFIG_RESVERVE_FBMEM_ADDR (CONFIG_DDR_BASE+CONFIG_RESVERVE_FBMEM_OFFSET)
//#define CONFIG_RESVERVE_FBMEM_MASK (~(CONFIG_RESVERVE_FBMEM_SIZE-1))
#endif