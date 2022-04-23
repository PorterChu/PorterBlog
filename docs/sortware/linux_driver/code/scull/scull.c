#include <linux/module.h>  
#include <linux/moduleparam.h>  
#include <linux/init.h>  
#include <linux/kernel.h> /* printk() */  
#include <linux/slab.h>       /* kmalloc() */  
#include <linux/fs.h>     /* everything... */  
#include <linux/errno.h>  /* error codes */  
#include <linux/types.h>  /* size_t */  
#include <linux/fcntl.h>  /* O_ACCMODE */  
#include <linux/cdev.h>  
#include <asm/system.h>       /* cli(), *_flags */  
#include <asm/uaccess.h>  /* copy_*_user */  
#include "scull.h"      /* local definitions */   
/*  
 * Our parameters which can be set at load time.  
 */  
//主设备号   
int scull_major =   SCULL_MAJOR;   
//次设备号   
int scull_minor =   0;   
//请求连续设备编号数量   
int scull_nr_devs = SCULL_NR_DEVS;  /* number of bare scull devices */  
//量子大小   
int scull_quantum = SCULL_QUANTUM;   
//量子集大小   
int scull_qset =    SCULL_QSET;   
module_param(scull_major, int, S_IRUGO);   
module_param(scull_minor, int, S_IRUGO);   
module_param(scull_nr_devs, int, S_IRUGO);   
module_param(scull_quantum, int, S_IRUGO);   
module_param(scull_qset, int, S_IRUGO);   
struct scull_dev *scull_devices;    /* allocated in scull_init_module */  
/*  
 * Empty out the scull device; must be called with the device  
 * semaphore held.  
 */  
/*  
 * 释放整个数据区，简单遍历列表并且释放它发现的任何量子和量子集。  
 * 在scull_open在文件为写而打开时调用。  
 * 调用这个函数时必须持有信号量。  
 */  
int scull_trim(struct scull_dev *dev)   
{   
    struct scull_qset *next, *dptr;   
    //量子集大小   
    int qset = dev->qset;   /* "dev" is not-null */  
    int i;   
    for (dptr = dev->data; dptr; dptr = next) { /* all the list items */  
        if (dptr->data) {//量子集中有数据   
            //遍历释放当前量子集中的每个量子，量子集大小为qset   
            for (i = 0; i < qset; i++)   
                kfree(dptr->data[i]);   
            //释放量子数组指针   
            kfree(dptr->data);   
            dptr->data = NULL;   
        }   
        //next获取下一个量子集，释放当前量子集   
        next = dptr->next;   
        kfree(dptr);   
    }   
    //清理struct scull_dev dev中的变量的值   
    dev->size = 0;   
    dev->quantum = scull_quantum;   
    dev->qset = scull_qset;   
    dev->data = NULL;   
    return 0;   
}   
/*  
 * Open and close  
 */  
int scull_open(struct inode *inode, struct file *filp)   
{   
    struct scull_dev *dev; /* device information */  
    dev = container_of(inode->i_cdev, struct scull_dev, cdev);   
    filp->private_data = dev; /* for other methods */  
    /* now trim to 0 the length of the device if open was write-only */  
    //文件以只读模式打开时，截断为0   
        if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {   
        if (down_interruptible(&dev->sem))   
            return -ERESTARTSYS;   
        scull_trim(dev); /* ignore errors */  
        up(&dev->sem);   
    }   
    return 0;          /* success */  
}   
int scull_release(struct inode *inode, struct file *filp)   
{   
    return 0;   
}   
/*  
 * Follow the list  
 */  
//返回设备dev的第n个量子集的指针，量子集不够n个就申请新的   
struct scull_qset *scull_follow(struct scull_dev *dev, int n)   
{   
    //第一个量子集指针   
    struct scull_qset *qs = dev->data;   
    /* Allocate first qset explicitly if need be */  
    // 如果当前设备还没有量子集，就显示分配第一个量子集   
    if (! qs) {   
        qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);   
        if (qs == NULL)   
            return NULL;  /* Never mind */  
        memset(qs, 0, sizeof(struct scull_qset));   
    }   
    /* Then follow the list */  
    // 遍历当前设备的量子集链表n步，量子集不够就申请新的   
    while (n--) {   
        if (!qs->next) {   
            qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);   
            if (qs->next == NULL)   
                return NULL;  /* Never mind */  
            memset(qs->next, 0, sizeof(struct scull_qset));   
        }   
        qs = qs->next;   
        continue;   
    }   
    return qs;   
}   
/*  
 * Data management: read and write  
 */  
ssize_t scull_read( struct file *filp, //设备对应的文件结构    
                    char __user *buf,  //读到用户空间   
                    size_t count,      //字节数   
                    loff_t *f_pos)     //要读的位置，在filp私有数据中的偏移   
{   
    struct scull_dev *dev = filp->private_data;    
    struct scull_qset *dptr;    /* the first listitem */  
    //量子、量子集大小   
    int quantum = dev->quantum, qset = dev->qset;   
    //一个量子集的字节数   
    int itemsize = quantum * qset; /* how many bytes in the listitem */  
    int item, s_pos, q_pos, rest;   
    ssize_t retval = 0;   
    if (down_interruptible(&dev->sem))   
        return -ERESTARTSYS;   
    //要读的位置超过了数据总量   
    if (*f_pos >= dev->size)   
        goto out;   
    //要读的count超出了size，截断count   
    if (*f_pos + count > dev->size)   
        count = dev->size - *f_pos;   
    /* find listitem, qset index, and offset in the quantum */  
    //在量子/量子集中定位读写位置：第几个量子集，中的第几个量子，在量子中偏移   
    //第几个量子集   
    item = (long)*f_pos / itemsize;   
    //在量子集中的偏移量   
    rest = (long)*f_pos % itemsize;   
    //第几个量子，在量子中的偏移   
    s_pos = rest / quantum; q_pos = rest % quantum;   
    /* follow the list up to the right position (defined elsewhere) */  
    //读取要读的量子集的指针   
    dptr = scull_follow(dev, item);   
    //读取出错处理   
    if (dptr == NULL || !dptr->data || ! dptr->data[s_pos])   
        goto out; /* don't fill holes */  
    /* read only up to the end of this quantum */  
    // 只在一个量子中读：如果count超出当前量子，截断count   
    if (count > quantum - q_pos)   
        count = quantum - q_pos;   
    // 将要读位置的内容复制count字节到用户空间buf中   
    if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count)) {   
        retval = -EFAULT;   
        goto out;   
    }   
    *f_pos += count;   
    retval = count;   
  out:   
    up(&dev->sem);   
    return retval;   
}   
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,   
                loff_t *f_pos)   
{   
    struct scull_dev *dev = filp->private_data;   
    struct scull_qset *dptr;   
    //量子、量子集大小   
    int quantum = dev->quantum, qset = dev->qset;   
    // 一个量子集总字节数   
    int itemsize = quantum * qset;   
    int item, s_pos, q_pos, rest;   
    ssize_t retval = -ENOMEM; /* value used in "goto out" statements */  
    if (down_interruptible(&dev->sem))   
        return -ERESTARTSYS;   
    /* find listitem, qset index and offset in the quantum */  
    //第几个量子集   
    item = (long)*f_pos / itemsize;   
    //在该量子集中的偏移   
    rest = (long)*f_pos % itemsize;   
    //在该量子集中的第几个量子，在量子中的偏移   
    s_pos = rest / quantum; q_pos = rest % quantum;   
    /* follow the list up to the right position */  
    //返回该量子集的指针   
    dptr = scull_follow(dev, item);   
    if (dptr == NULL)   
        goto out;   
    //如果该量子集数据为NULL，就申请一块新内存   
    if (!dptr->data) {   
        dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);   
        if (!dptr->data)   
            goto out;   
        memset(dptr->data, 0, qset * sizeof(char *));   
    }   
    //如果第s_pos个量子是NULL，就申请一块新内存   
    if (!dptr->data[s_pos]) {   
        dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);   
        if (!dptr->data[s_pos])   
            goto out;   
    }   
    /* write only up to the end of this quantum */  
    // 只在一个量子中写，如果count超出当前量子就截断   
    if (count > quantum - q_pos)   
        count = quantum - q_pos;   
    //从用户空间拷贝数据到内核空间，失败返回没有拷贝的字节数，成功返回0   
    if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count)) {   
        retval = -EFAULT;   
        goto out;   
    }   
    *f_pos += count;   
    retval = count;   
    /* update the size */  
    // 更新字节总数大小   
    if (dev->size < *f_pos)   
        dev->size = *f_pos;   
  out:   
    up(&dev->sem);   
    return retval;   
}   
  
struct file_operations scull_fops = {   
    .owner =    THIS_MODULE,   
    .read =     scull_read,   
    .write =    scull_write,   
    .open =     scull_open,   
    .release =  scull_release,   
};   
/*  
 * Finally, the module stuff  
 */  
/*  
 * The cleanup function is used to handle initialization failures as well.  
 * Thefore, it must be careful to work correctly even if some of the items  
 * have not been initialized  
 */  
void scull_cleanup_module(void)   
{   
    int i;   
    //主次设备号合成一个dev_t结构，即设备编号   
    dev_t devno = MKDEV(scull_major, scull_minor);   
    /* Get rid of our char dev entries */  
    if (scull_devices) {   
        //便利释放每个设备的数据区   
        for (i = 0; i < scull_nr_devs; i++) {   
            //释放数据区   
            scull_trim(scull_devices + i);   
            //移除cdev   
            cdev_del(&scull_devices[i].cdev);   
        }   
        //释放scull_devices本身   
        kfree(scull_devices);   
    }   
    /* cleanup_module is never called if registering failed */  
    unregister_chrdev_region(devno, scull_nr_devs);   
}   
  
/*  
 * Set up the char_dev structure for this device.  
 */  
// 建立char_dev结构   
static void scull_setup_cdev(struct scull_dev *dev, int index)   
{   
    int err, 
    devno = MKDEV(scull_major, scull_minor + index);   
       
    cdev_init(&dev->cdev, &scull_fops);   
    dev->cdev.owner = THIS_MODULE;   
//  dev->cdev.ops = &scull_fops;   
    //添加字符设备dev->cdev,立即生效   
    err = cdev_add (&dev->cdev, devno, 1);   
    /* Fail gracefully if need be */  
    if (err)   
        printk(KERN_NOTICE "Error %d adding scull%d", err, index);   
}   
  
int scull_init_module(void)   
{   
    int result, i;   
    dev_t dev = 0;   
/*  
 * Get a range of minor numbers to work with, asking for a dynamic  
 * major unless directed otherwise at load time.  
 */  
    //申请设备编号，若在加载时没有指定主设备号就动态分配   
    if (scull_major) {   
        dev = MKDEV(scull_major, scull_minor);   
        result = register_chrdev_region(dev, scull_nr_devs, "scull");   
    } else {   
        result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs,   
                "scull");   
        scull_major = MAJOR(dev);   
    }   
    if (result < 0) {   
        printk(KERN_WARNING "scull: can't get major %d\n", scull_major);   
        return result;   
    }   
        /*   
     * allocate the devices -- we can't have them static, as the number  
     * can be specified at load time  
     */  
    //给scull_dev对象申请内存   
    scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);   
    if (!scull_devices) {   
        result = -ENOMEM;   
        goto fail;  /* Make this more graceful */  
    }   
    memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));   
        /* Initialize each device. */  
    for (i = 0; i < scull_nr_devs; i++) {   
        scull_devices[i].quantum = scull_quantum;   
        scull_devices[i].qset = scull_qset;   
        //初始化互斥锁，把信号量sem置为1   
        sema_init(&scull_devices[i].sem, 1);       
        //init_MUTEX(&scull_devices[i].sem);   
        //建立char_dev结构   
        scull_setup_cdev(&scull_devices[i], i);   
        }   
    return 0; /* succeed */  
  fail:   
    scull_cleanup_module();   
    return result;   
}   
module_init(scull_init_module);   
module_exit(scull_cleanup_module);   
MODULE_AUTHOR("Tekkamanninja");   
MODULE_LICENSE("Dual BSD/GPL");  