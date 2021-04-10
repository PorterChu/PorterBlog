## 字符设备驱动

### 静态申请设备号函数

```c
#define MINORBITS 20
#define MINORMASK ((1U << MINORBITS) - 1)
#define MAJOR(dev) ((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev) ((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi) (((ma) << MINORBITS) | (mi))

/*
* param：
*       from:向内核申请的设备号
*       count:向内核申请的设备号数目
*       name:向内核申请的设备号名称(通过cat /proc/devices查看)
* return:
*       0:success, or means failed 
* remark:
*       MKDEV:设备号(包括主设备号+次设备号，主设备号<<20 + 次设备号，4字节32bit中前12bit为唯一主设备号，后20bit为次设备号，次设备号从0开始)
*       MAJOR:主设备号
*       MINOR:次设备号
*/

int register_chrdev_region(dev_t from, unsigned count, const char *name);

/*
* example
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fs.h>

#define demo_MAJOR	168
#define demo_MINOR	0
#define demo_COUNT	1

dev_t dev = 0;

int __init demo_init(void)
{
    int ret = 0;
    //生成设备号
    dev = MKDEV(demo_MAJOR,demo_MINOR);
    //注册设备号
    ret = register_chrdev_region(dev,demo_COUNT,"demo_test");
    if (ret < 0)
    {
        printk("reigster_chrdev_region failed!\n");
        goto failure_register_chrdev;
    }
    printk("register_chrdev_region success!\n");
    return 0;

failure_register_chrdev:
    return ret;
}

void __exit demo_exit(void)
{
	//注销设备号
	unregister_chrdev_region(dev, demo_COUNT);
}

module_init(demo_init);
module_exit(demo_exit);
```

### 动态申请设备号函数

```c
/*
* param:
*       dev:向内核申请的设备号
*       baseminor:向内核申请次设备号的起始，通常为0
*       count:向内核申请设备号的数目
*       name:向内核申请的设备号名称(通过cat /proc/devices查看)
* return:
*       小于0则failed
*/

int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count, const char *name);
```

### 设备号释放函数

```c
/*
* param:
*       from:要释放的第一个设备号
*       count:要释放的设备号数量
*/

void unregister_chrdev_region(dev_t from, unsigned count);
```