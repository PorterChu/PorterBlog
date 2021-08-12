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

```shell
cat /proc/sys/kernel/printk                   # 查看printk输出等级
echo 0  4  0  7 > /proc/sys/kernel/printk     # 调整printk输出等级
```