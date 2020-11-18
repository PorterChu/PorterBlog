## Makefile介绍

Makefile是用来编译大型软件工程的，Makefile对语法格式非常在意，很多语法格式和shell编程类似，Makefile写好配置后，在linux下只需要执行make就可自动进行编译，详细的Makefile写作细节可参考陈皓大神的《跟我一起写Makefile》。

## Makefile提炼

1. 通用格式

```Makefile
目标：依赖
    命令
.PHONY:clean
clean:
    -rm 文件
```

注意：
- 命令之前的空格是1个tab键，不能以4个空格键代替，否则会报错
- 通常依赖的文件有很多，可以用`\`进行换行输入
- .PHONY:clean这句指令可以避免伪目标clean与目标重名
- rm指令前添加`-`，可以忽略有问题的文件，继续执行后续的清除指令


@静默执行

```Makefile
# Allow for silent builds 静默编译
ifeq (,$(findstring s,$(MAKEFLAGS)))
XECHO = echo
else
XECHO = :
endif
```

?=变量未赋值(未定义)则进行赋值，变量已赋值就忽略赋值

+=给已经赋值的变量续接赋值，中间会有一个空格隔开，类似C语言的strcat

:=通常也为赋值，但往前找
=赋值，但往后找

通配符：*、？、[]、%(规则通配符)

自动变量：$@(目标文件名) 、 $<(目标依赖名)、 $^(依赖的文件集合)、@？

subst filter函数

gcc -M main.c执行后可以自动推导并列出编译main.c所需要的头文件

gcc -MM main.c执行后可以自动推导并输出非C标准库以外的头文件

wildcard 关键字

vpath关键字：指定文件搜索目录
1. vpath %.h ../include  //指定在../include路径下查找所有.h的文件
2. vpath %.h             //清除符合%.h文件的../include搜索目录
3. vpath                 //清除所有设置好的文件搜索目录


make —help
