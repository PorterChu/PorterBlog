## ARRAY_SIZE宏

- 路径：kernel/include/linux/kernel.h
- 作用：求设备结构体中设备的个数

```c
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
```

`sizeof(arr)/sizeof((arr)[0])`是求出设备的个数，`__must_be_array(arr)`是防止被吴用，比如用指针而不是数组上。