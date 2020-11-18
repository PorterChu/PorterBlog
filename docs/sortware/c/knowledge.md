## 扩展之NULL

当我们在定义一个指针的时候，经常用NULL来给指针赋初值，同时在应用指针的时候也会用NULL来进行指针内容的判断，那NULL究竟是什么？下面一起来探讨下。

### 1. NULL定义

NULL不是C语言的关键字，它是一个宏，定义如下：在C++中NULL等同于0，在C语言中NULL表示void *类型的0，其实本质还是0。

```c
#ifdef _cplusplus
#define NULL 0
#else
#define NULL (void *)0
#endif
```

### 2. NULL使用

如上面介绍，NULL的使用惯例是为指针赋初值，代表着指针指向0x0000，通常在内存的区域划分中，以0x0000为基地址的区域是一个受保护的区域，一般程序无法访问去篡改其中的数值，所以将这个基地址赋值给新定义的指针，可以保证这个指针不会胡乱指向别的空间篡改了数值造成程序错误，如果不给指针赋初值，那这个定义的指针就叫做`野指针`，野指针可以指向内存任意地址空间，在程序运行中会造成无法预料的问题：

```c
int *p = NULL;                      //定义int型指针p，并赋值NULL
p = (int *)malloc(sizeof(int)*5);   //为指针p分配20字节空间
if(NULL == p)                       //判断p是否为0
```

注意上面用NULL来判断p是否为0的时候，写成`NULL == p`而不是`p == null`，这是为了防止我们疏忽将判断等式写成了赋值表达式`p = NULL`，程序在编译过程中是不会报错出警告的，这就加重了调试的困难度。

### 3. NULL比较

既然NULL表示0，为了同其他字符以示区分，这边来对如下字符进行比较：

```c
'\0'：是一个转义字符，对应ASCII码的值是0(十进制)，本质是0；
'0'：是一个字符，对应ASCII码的值是48(十进制)，本质是48；
0：是一个数字0，本质就是0；
NULL：是一个表示式，是强制类型转换为void *类型的0，本质也是0。
```

总结： 
- '\0'用法是C语言字符串的结尾标志，一般放在字符串最后，用来判断字符串是否已经结束；
- '0'是字符0，一般用来获取0对应的ASCII码值；
- 0是数字0，用来判断int类型的变量是否为0；
- NULL是一个表达式，一般用来比较指针是否为野指针。

## 扩展之强制类型转换

程序中经常使用强制类型转换将不同类型的变量转换成我们需要的类型变量，那类型转换在内存中如何体现呢？通过下面的例子来看下：

```c
#include<stdio.h>
int main(void)
{
    float a = 12.34;
    int b = (int)a;
    printf("b = %d\n",b);
    printf("a = %f\n",a);
    return 0;
}
```

程序执行结果：b = 12, a = 12.34

很简单的示例，通过将float型变量a强制转换成int型后赋值给变量b，思考下程序已经对a做了强制转换，为什么打印出来a的值还是12.34呢？再来看一个例子：

```c
#include<stdio.h>
int main(void)
{
    int a = 10;
    int b = 3;
    float d = 0;
    d = (float)a/b;
    printf("d = %f\n",d);
    return 0;
}
```

程序执行结果：d = 3.0000

结果为什么是3.0000而不是3.3333呢？

![](image/类型转换.svg)

如图介绍，在内存中类型转换并不是一步完成的，而是分成了4步来完成：

- 第一步：在栈区创建一个临时变量x，类型与d的类型一致，也是float型；
- 第二步：因为a和b的类型都是int型，所以a/b的结果也是int型取整为数值3，将3赋值给临时变量x；
- 第三步：将临时变量x的值赋给变量d，因为是float型，所以要取四位小数，所以d的值为3.0000；
- 第四步：在栈区销毁临时变量x。

总结：当对一个变量或表达式进行强制类型转换，内存中会创建一个临时变量来存储计算后的数值，并将这个数值赋给对应的变量，而内存中实际存储的值并没有进行改动，程序执行完会自动销毁这个临时变量。

## 扩展之DEBUG宏

### 1. DEBUG宏意义

DEBUG宏在软件DEBUG版本和RELEASE版本上应用较多，通常DEBUG版本打印出错log定位问题，而在RELEASE版本发布中则不需要此类log，因为程序在进行log打印的时候会占用很多系统资源，拖慢操作系统运行效率，因此DEBUG版本的代码效率是远远低于RELEASE版本的。

### 2. DEBUG宏实现原理

DEBUG宏简单的实现原理如下：

```c
#define DEBUG   //确认是否需要注释
#ifdef DEBUG
#define dbg() printf()
#else   
#define dbg() 
#endif
```

DEBUG版本和RELEASE版本控制区分是通过`#define DEBUG`这个宏定义来区分的，这个宏定义是程序员自己定义的，当程序编译DEBUG版本进行代码调试时，需要提前加上此宏定义，而后代码中的所有dbg()语句都会替换成printf()语句，将log信息按照dbg函数中预先设置好的格式打印出来；而当编译RELEASE版本时，则需要注释掉此宏定义，代码中的dgb()语句会全部替换成空，所有的log信息均不打印。

### 3. DEBUG宏实际应用

```c
#define DEBUG
#ifdef DEBUG    
#define DBG(...) /    
do{ /    
    fprintf(stdout, "[DEBUG  ]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); /    
    fprintf(stdout, __VA_ARGS__); /    
}while(0)    
#else    
#define DBG(...)    
#endif 

#define ERROR(...) /    
do{ /    
    fprintf(stderr, "[ERROR  ]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); /    
    fprintf(stderr, __VA_ARGS__); /    
}while(0)    

#define WARNING(...) /    
do{ /    
    fprintf(stdout, "[WARNING]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); /    
    fprintf(stdout, __VA_ARGS__); /    
}while(0)    

#define INFO(...) /    
do{ /    
    fprintf(stdout, "[INFO  ]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); /    
    fprintf(stdout, __VA_ARGS__); /    
}while(0)    

#define SHOW_TIME(...) /    
do{ /    
    extern unsigned long long gLatestTime;/    
    timeval tp;/    
    gettimeofday(&tp, NULL);/    
    unsigned long long now = tp.tv_sec*1000000+tp.tv_usec; /    
    if(gLatestTime != 0) /    
    { /    
        fprintf(stdout, ">>>>>>>>>Used Time: %s[%d], %s: %ld.%ld, %llu ms ", __FILE__, __LINE__, __func__, tp.tv_sec, tp.tv_usec, (now-gLatestTime)/1000);/    
        fprintf(stdout, __VA_ARGS__); /    
        fprintf(stdout, "/n"); /    
    } /    
    gLatestTime = now;/    
}while(0)    
```

上方示例中实现方式与原理介绍中的一样，对于编译DEBUG版本，需要添加`#define DEBUG`，代码中会将DBG()、ERROR()、WARNING()、INFO()、SHOW_TIME()替换成对应的格式打印出来；而编译发布RELEASE版本时，则注释掉`#define DEBUG`，则这些打印语句会被替换成空，所有的log都不会被打印。

## 扩展之可变参函数

### 1. 可变参函数说明

C语言中很多函数在定义时使用了可变参，可变参顾名思义表示函数的参数个数是不定的，程序员在使用这些函数时就可以根据需要来进行传参设定。通过man手册来看下printf函数的定义，如下：

```c
int printf(const char *format, ...);
```

参数format表示格式字符串的指令，是一个确定的参数，因为函数至少需要一个固定的参数；`...`表示可选参数，调用时传递给`...`的参数可有可无，根据实际情况而定。

### 2. 实现可变参函数

要实现一个可变参函数需要使用到头文件`stdarg.h`中定义的这些函数：

- 可变参数函数定义

```c
typedef char * va_list;                  //在strarg.h头文件中定义，表示指向个数可变的参数列表指针的一种类型
void va_start(va_list ap, last);         //va_start()使参数列表指针ap指向参数列表中的第一个可选参数，last是可变参数`...`之前的最后一个固定参数
type va_arg(va_list ap, type);           //返回参数列表中指针ap所指的参数，返回类型为type，并使指针ap指向参数列表中下一个参数
void va_copy(va_list dest, va_list src); //dest，src的类型都是va_list，va_copy()用于复制参数列表指针，将dest初始化为src
void va_end(va_list ap);                 //清空参数列表，并置参数指针ap无效
```

注意：指针ap被置无效后，可以通过调用va_start()、va_copy()恢复ap，每次调用va_start()或者va_copy()后，必须得有相应的va_end()与之匹配；参数指针可以在参数列表中随意的来回移动，但必须在va_start() ~ va_end()之内。

- 可变参数宏定义

```c
#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))    //求变量占用内存空间大小，便于代码移植
#define va_start(ap,v) (ap = (va_list)&v + _INTSIZEOF(v))                     //第一个可选参数地址
#define va_arg(ap,t) (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))          //返回当前参数，并指向下一个参数地址
#define va_end(ap) (ap = (va_list)0)                                          //将指针置为无效
```

以int TestFunc(int n1, int n2, int n3, …)为例子，来看参数传递时的内存堆栈实际情况：

![](./image/va可变参.svg)

1. 通过va_start得到第一个可选参数的地址：

```c
&n3 + _INTSIZEOF(n3)    //最后一个固定参数的地址 + 该参数占用内存的大小
```

2. va_arg身兼二职：返回当前参数，并使参数指针指向下一个参数：

```c
#define va_arg(ap,t) (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
/* 指针ap指向下一个参数的地址 */
1．  ap += _INTSIZEOF(t)
/* ap减去当前参数的大小得到当前参数的地址，再强制类型转换后返回它的值 */
2．  return *(t *)( ap - _INTSIZEOF(t))
```

注意：函数的固定参数部分，可以直接从函数定义时的参数名获得；对于可选参数部分，先将指针指向第一个可选参数，然后依次后移指针，根据与结束标志的比较来判断是否已经获得全部参数。因此，va函数中结束标志必须事先约定好，否则指针会指向无效的内存地址，导致出错。

- _INTSIZEOF宏介绍

```c
#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1)) 
```

对于指针在可变参之间的偏移量，也就是上一个可变参和下一个可变参在内存空间的间隔大小是不确定的，这里涉及到内存对齐的问题，而_INTSIZEOF宏是就是为了此问题，内存对齐跟硬件平台有很大的关系，_INTSIZEOF(n)最终大小肯定是sizeof(int)的整数倍。

- VA可变参示例：

```c
#include <stdio.h>
#include <string.h>  
#include <stdarg.h>  

/* ANSI标准形式的声明方式，括号内的省略号表示可选参数 */  
void demo(char *msg, ...)  
{  
    va_list argp;                          /* 定义保存函数参数的结构 */  
    int argno = 0;                         /* 纪录参数个数 */  
    char *para;                            /* 存放取出的字符串参数 */                                      
    va_start(argp, msg);                   /* argp指向传入的第一个可选参数，msg是最后一个确定的参数 */  
 
    while(1) 
    {  
        para = va_arg(argp, char *);       /* 取出当前的参数，类型为char *. */  
        if( strcmp(para, "\0") == 0 )      /* 采用空串指示参数输入结束 */  
            break;  
        printf("Parameter #%d is: %s\n", argno, para);  
        argno++;  
    }  
	
    va_end(argp);                          /* 将argp置为NULL */  
}

int main(void)  
{  
    demo("DEMO", "This", "is", "a", "demo!" , "\0");  
	return 0;
}    
```

## 扩展之预定义宏

通常在调试代码时经常看到如下的宏，这些是编译器定义的宏，宏的名称都是以两个下划线开始且以两个下划线结束：

```c
__cplusplus    //此宏通常与extern "C"一起使用，目的是告诉编译器这是一段用C写的代码，需要用C的方式进行链接
__DATE__       //显示当前源文件的编译日期，日期格式是按照Mmm dd yyyy(如：Mar 192006)来显示 
__FILE__       //显示当前源文件的名称，用%s来打印
__LINE__       //显示当前指令所在的整数行号，用%d来打印
__TIME__       //显示具体的编译时间，格式为hh：mm：ss(如：08：00：59)
__FUNCTION__   //显示当前指令所在的函数名，用%s来打印
```

## 扩展之inline函数

函数调用时会在栈上为函数变量分配空间，频繁的函数调用会造成栈空间不足而导致程序出错的问题，如死循环的递归调用执行的最终结果就是栈内存空间枯竭。为解决此类问题，引入了inline修饰符，即为内联函数。

- 示例：

```c
#include <stdio.h>
inline char* test(int a) 
{
    return (i % 2 > 0) ? "奇" : "偶";
} 

int main(void)
{
    int i = 1;
    for(; i < 100; i++) 
    {
        printf("i:%d，奇偶性:%s\n", i, test(i));    
    }
    return 0;
}
```

示例中，inline内联函数会在for循环中任何调用test函数的地方都换成(i % 2 > 0) ? "奇" : "偶"，这样就避免了频繁调用test函数对栈内存重复开辟所带来的消耗。

- 使用说明：
  - inline内联函数只适合功能简单的函数使用，当被调用函数体内代码较长或者有循环、递归调用时，内联函数反而会增大栈内存空间的消耗，替换后代码量多了，占用的栈内存也多了；
  - 适当的使用inline内联函数会提高代码执行效率，inline是将函数的每次调用都替换成函数本体，这是以空间换时间的做法，因为函数调用需要将之前的参数以栈的形式保存起来，调用结束后又要从栈中恢复那些参数，所以内联函数的替换节省了函数调用的开销从而提高执行效率；
  - inline是关键字，必须放在函数定义前才能有效，仅放在声明前无效，通常选择仅在函数定义体前使用inline；
  - inline函数的定义一般放在头文件中，无需再.c文件中重复实现；
  - inline函数仅仅是对编译器的建议，最后是否执行内联展开由编译器决定。


## 扩展之避免二义性

当一个工程中多个.c文件都包含了同一个头文件时，如果进行编译会报重复定义的错误，此时有两种方式可以避免。

方式一：使用`#ifndef`，格式如下

```c
#ifndef _SOMEFILE_        //头文件标识，字符全部大写，符号`.`用下划线`_`替代，如stdio.h写成_STDIO_H_
#define _SOMEFILE_        //头文件标识，字符全部大写，符号`.`用下划线`_`替代，如stdio.h写成_STDIO_H_
... ...                   //声明语句，通常是extern函数声明
#endif
```

- 优点：
  - 受C/C++语言标准支持，不受编译器限制，移植性好
  - 既能保证在一个文件中不会被包含多次，也能保证内容完全相同的两个文件不会被同时包含
- 缺点：
  - 若不同的头文件宏名相同，可能导致头文件明明存在，编译器提示找不到声明
  - 需要原地展开头文件判定是否重定义，编译时间较长

方式二：使用`#pragma once`，格式如下

```c
#pragma once
... ...                    //声明语句，通常是extern函数声明
```

- 优点：
  - 不会出现头文件宏名相同导致编译器找不到头文件
  - 编译速度快，时间较短
- 缺点：
  - 无法对头文件中部分代码块进行声明，作用范围只能针对整个文件
  - 受编译器限制，部分编译器无法识别，兼容性差

## 扩展之#pragma用法

预处理指令#pragma功能很多，相对也较为复杂，它的作用是设定编译器的状态或指示编译器完成一些特定的动作，也就是说#pragma指令的功能依赖具体的编译器，每个编译器能识别的#pragma指令不同，如果代码中使用较多的#pragma指令会令代码可移植性大大降低。下面梳理了常用的几个#pragma指令：

- #pragma message

此指令可以让编译器输出相应的信息，尤其当代码中设置了较多的宏后，可以通过输出信息来进行检查：

```c
#ifdef _X86  
#pragma message("_X86 macro activated!")    //通过打印的_X86 macro activated!信息来确认_X86宏已经被定义
#endif
```

- #pragma code_seg([[{push|pop},][identifier,]]["segment-name"[,"segment-class"]])

此指令可以设置程序中函数代码存放的位置，code_seg后面跟着的参数都是可选参数，如果code_seg不带参数则存放在默认的.text代码段中：

```c
#pragma code_seg      //不带参数
void func1{}          //表示将func1的函数代码存放在默认的.text中

#pragma code_seg(".my_data1") 
void func2{}          //表示将func2的函数代码存放在设定的.my_data1中

#pragma code_seg(push, r1, ".my_data2")  //push是压入，r1是标识符
void func3(){}        //表示将标识符为r1的func3的函数代码存放在.my_data2中
```

- #pragma once

此指令通常放在头文件中，作用域为整个头文件，功能是保证此头文件只被编译一次，功能类似#ifndef，详细可参考[扩展之避免二义性](#扩展之避免二义性)。

- #pragma warning

此指令表示允许对编译器warning信息进行有效处理：

```c
#pragma warning(disable: 4507)       //不显示4507号警告信息  
#pragma warning(once: 4385)          //仅报告一次4385号警告信息
#pragma warning(error: 164)          //把164号警告信息作为一个错误
#pragma warning(push, n)             //把所有警告信息的现有警告状态保存起来，并且把全局警告等级设定为n(n范围为1-4，数字越低等级越高)    
#pragma warning(pop)                 //从栈中弹出最后一个警告信息，在入栈和出栈之间所作的一切改动取消
```

- #pragma pack([[{push|pop},][identifier,]][n])

此指令用于指定内存对齐原则，同样的数据类型在不同系统下内存空间占用的字节大小是不一样的，如下表所示：

|type|char|bool|short|int|unsigned int|float|long|double|long long|long double|指针|
|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
|32bit编译器|1|1|2|4|4|4|4|8|8|8|4|
|64bit编译器|1|1|2|4|4|4|8|8|8|8|8|

通过#pragma pack指令就可以指导编译器将结构体成员按照特定的对齐原则在内存空间中进行排列，其中字节不足的补空字节，示例如下：

```c
#pragma pack(n)          //n可以设置为1、4、8、16等字节，不同的数字按照不同的字节来对齐，最终对齐的字节取决于设定的n值和结构体中最大数据成员字节数较小的一方
#pragma pack()           //取消自定义对齐方式，恢复默认对齐
#pragma pack(push)       //按照指定的对齐字节压入
#pragma pack(pop)        //按照指定的对齐字节弹出
#pragma pack(push,4)     //等同于#pragma pack(push)和#pragma pack(4)的组合，作用是按照4字节对齐方式压入内存中存放
__attribute(aligned(n))  //结构体成员以n字节来对齐，如果结构中有成员的字节长度大于n，则按照最大成员的长度来对齐，通常__attribute(aligned(n))放在结构体定义的最后
__attribute((packed))    //取消结构在编译过程中的优化对齐，按照实际占用字节数进行对齐
```