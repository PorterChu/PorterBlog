## 1. 字符串越界

Q：请问下面代码有什么问题？ 

```c
int main()
{
	char a;
	char *str = &a;
	strcpy(str,"hello");
	printf(str);    //忽略printf函数字符串传参格式警告，正确的写法printf("%s\n",str);
	return 0;
}
```

执行结果：Segmentation fault(段错误)

**分析：a是一个字符，在栈上占用1个字节，str是一个字符指针，str指向的地址为a的栈空间地址，字符串`hello`包括结尾空字符`'\0'`共计占6个字节，所有strcpy函数将`hello`复制给str时已经超出a的栈空间大小，所以执行结果为Segmentation fault(段错误)。**

修改如下：

```c
int main()
{
	char a[6];
	char *str = a;
	strcpy(str,"hello");
	printf("%s\n",str);
	return 0;
}
```

注意：当定义字符数组a空间小于6字节时，打印出的结果依旧可以输出'hello',这是因为内存空间是连续的，虽然栈上给a分配的空间小于6字节，但strcpy会把所有的字符复制进去，在实际应用中是有风险的，会覆盖掉部分地址空间中的内容，造成内存泄漏。

## 2. strlen与sizeof

Q：输出是？

```c
int main(void)
{	
	char a[10];
    printf("%ld\n",sizeof(a));
	printf("%ld\n",strlen(a));
    return 0;
}
```

执行结果：10 3

分析：sizeof()是输出实际的字节大小，strlen()是输出字符串中除结尾`'\0'`以外其他字符的个数，题目中字符数组a没有赋初值，strlen()输出的值是不正确的。

## 3. 用指针访问内存地址

```c
int *p = (int *)0x30080000;     //进行内存地址强制类型转换，定义指针p进行访问此地址
*p = 16;                        //对指针p进行解引用后进行赋值，修改内存地址中存储的变量值
```