## 1. main函数

一个程序的入口是main函数，main函数有两种标准写法，这两种区别在于是否需要对main函数进行传参：

```c
int main(void)                     //main函数不传参
int main(int argc, char *argc[])   //main函数传参，函数也可定义为：int main(int argc, char **argv)
```

在阐述这部分之前，先来探讨下linux下main函数执行的本质。我们知道其他函数都是直接或间接被main函数调用，那main函数是被谁调用的呢？这就涉及到进程的概念了，

fork函数
exec函数族

## 2. main函数之不传参

不对main函数传参的话，直接在函数内添加void即可，函数体内执行的结果就与外界传参无任何关联，如果像控制程序执行，必须修改程序代码再重新编译。那函数中的void是什么含义呢？

### 2.1 类型介绍

编程语言分为强类型语言和弱类型语言，C语言就是强类型语言，而脚本语言如makefile或shell都属于弱类型语言，两者很大的区别在于类型的限定。在强类型语言中变量都有自己明确的类型，因为一个变量要对应内存中的一段内存，编译器需要这个类型来确定在内存中占用多大的字节和这段内存的解析方法，如int型变量，在内存中占用4个字节，这4个字节是用int类型去解析。而弱类型语言是没有类型这种明确划分的，所有的变量都是一种类型，一般都是字符串，在使用的时候再去根据需要来处理变量。

值得一提的是，在C语言中，假设同一个变量在内存中分配的首地址都是相同的(0x0000)，不同的类型会决定内存地址的长度不同，int型会占用4个字节，则存储地址为0x0000 ~ 0x0003，char型会占用1个字节，存储地址为0x0000 ~ 0x0001，double型会占用8个字节，存储地址为0x0000 ~ 0x0007。同时，int型和float型占用的字节数相同，都是0x0000 ~ 0x0003的4个字节，但对于这一段内存长度的空间解析方式是不同的，int型空间按int型方式去解析，float型按float型方式去解析，如果用int型方式去解析float型内存空间，解析出来的值很大可能就不正确。

注意：变量有明确的类型，内存空间没有类型划分。这是因为在内存空间中，无论存储着int型还是double型变量，对于内存来说都只是一个数值，而对于变量来说，是需要按int型解析还是double型解析，这是不同的。

### 2.2 void类型

很多教程上将void解释为空，void类型解释为空类型或无类型，其实这是不正确的，void实际代表不确定的，void类型也就是还未明确的类型，以malloc函数为例，函数定义如下：

```c
#include <stdlib.h>
void *malloc(size_t size);
```

我们在给一个指针变量p分配一个size大小的空间时，并不知道这个指针变量是什么类型，可能是int型指针，也可能是char型指针，所以函数定义的时候用了void类型，等真正应用的时候，根据指针的变量再强制转换成对应的类型，如下给int型指针变量分配4字节内存：

```c
int *p = NULL;
p = (int *)malloc(sizeof(int));
```

所以，void类型都是事先并不确定类型，最终归宿也是被强制转换成其他类型。

## 3. main函数之传参

与上面不同，main函数传参可控制传参的内容，同时不需要重新编译，从而控制main函数执行的结果。

### 3.1 argc和argv

argc的类型是int型，说明argc是一个int型变量，表示程序运行时给main函数传递了几个参数；argv的类型是char *型，说明argv是一个char *型的字符串数组，这个数组中存放着多个字符串，每个字符串都是给main函数传参的实际内容，数组个数就是argc，argv[0]是给main函数传递的第一个参数，argv[1]是给main函数传递的第二个参数，以此类推。我们来看个例子，在test.c的文件内输入如下内容：

```c
#include<stdio.h>
int main(int argc, char *argv[])
{
    int i;
    printf("argc的个数是%d\n",argc);
    for(i=0;i<argc;++i)
    {
        printf("第%d个数的传参是%s\n",argc,argv[i]);
    }
    return 0;
}
```

编译后输入`./a.out`得到如下结果：

```c
argc的个数是1
第1个数的传参是./a.out
```

总结：在没有对main函数传递参数的时候，程序执行反馈的结果仍然有一个传参，传参的内容就是我们输入的`./a.out`命令。

输入`./a.out 123   345`得到结果：

```c
argc的个数是3
第3个数的传参是./a.out
第3个数的传参是123
第3个数的传参是345
```

总结：main函数实际传递的参数是从第二个开始计数，传递的参数内容就是在./a.out命令后面输入的内容，传递的参数用空格来间隔，多个间隔也仅会当一个空格来处理。

### 3.2 传参说明

通过一个简单的例子来说明main函数如何通过传参来控制程序输出：

```c
#include<stdio.h>
#include<string.h>
int main(int argc, char *[])
{
    if(argc != 2)
    {
        printf("we need to two param.\n");
        return -1;
    }
    if(!strcmp(argv[1],"0"))
    {
        printf("girl.\n");
    }
    else if(!strcmp(argv[1],"1"))
    {
        printf("boy.\n");
    }
    else
        printf("Unknowd.\n");
    return 0;
}
```

输入`./a.out 0`或者`./a.out 1`来控制程序执行的输出。

```c
porter@ubuntu:~/windows_share$ ./a.out 0
girl.
porter@ubuntu:~/windows_share$ ./a.out 1
boy.
```

这边需要注意的是：main函数传参时一定要检验argc，否则会造成argv字符串数组溢出出现段错误。

## 4. 扩展之NULL

NULL的宏定义

#ifdef _cplusplus
#define NULL 0
#else
#define NULL (void *)0
#endif

野指针
悬空指针

NULL使用惯例

if(NULL == P) 而不要写成 if(p == NULL)

\0 '0' 0 和NULL区别