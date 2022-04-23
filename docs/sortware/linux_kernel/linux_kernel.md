## ARRAY_SIZE宏

- 路径：kernel/include/linux/kernel.h
- 作用：求设备结构体中设备的个数

```c
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
```

`sizeof(arr)/sizeof((arr)[0])`是求出设备的个数，`__must_be_array(arr)`是防止被误用，比如用在指针而不是数组上。

## printk优先级

```c
int console_printk[4] = {
	CONSOLE_LOGLEVEL_DEFAULT,	/* console_loglevel */
	MESSAGE_LOGLEVEL_DEFAULT,	/* default_message_loglevel */
	CONSOLE_LOGLEVEL_MIN,		/* minimum_console_loglevel */
	CONSOLE_LOGLEVEL_DEFAULT,	/* default_console_loglevel */
};
```

在用户虚拟文件系统中设置\查看prink优先级：

```shell
cat /proc/sys/kernel/printk                   # 查看printk输出等级
echo 0  4  0  7 > /proc/sys/kernel/printk     # 调整printk输出等级
```

## devm_kzalloc

devm_kzalloc申请内存内存时，内部使用slab原理进行内部的分配，内存分配和回收受slab监管。同时申请内存时需要进行gfp的flag传参，当使用GFP_ KERNEL来申请内存时，若暂时不能满足，则进程会睡眠等待页，即会引起阻塞，因此不能在中断上下文或持有自旋锁的时候使用GFP_KERNE申请内存。