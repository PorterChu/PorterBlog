## Makefile介绍

Makefile是用来编译大型软件工程的，Makefile对语法格式非常在意，很多语法格式和shell编程类似，Makefile写好配置后，在linux下只需要执行make就可自动进行编译，详细的Makefile写作细节可参考陈皓大神的《跟我一起写Makefile》。

## Makefile提炼

1. 通用格式

```Makefile
目标：依赖
    命令
.PHONY:clean
clean:
    命令
```

注意：
- 命令之前的空格是1个tab键，不能以4个空格键代替，否则会报错
- 通常依赖的文件有很多，可以用`\`进行换行输入
- .PHONY:clean这句指令强调了clean是伪目标


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

自动变量：$@(目标文件名) 、 $<(目标依赖名)、 $^(依赖的文件集合)
