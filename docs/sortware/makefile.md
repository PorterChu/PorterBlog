## Makefile介绍

Makefile是用来编译大型软件工程的，Makefile对语法格式非常在意，很多语法格式和shell编程类似，详细的Makefile写作细节可参考陈皓大神的《跟我一起写Makefile》。

## Makefile提炼

Makefile写好配置后，在linux下只需要执行make就可自动进行编译，linux的make指令会在当前目录下寻找Makefile或makefile文件进行编译。

### 通用格式

```Makefile
目标：依赖
    @指令
.PHONY:clean
clean:
    -rm 文件
```

注意：
- 执行的gcc指令之前必须是1个tab键，不能以4个空格键代替，否则会报错
- 在gcc指令之前添加`@`后，Makefile就会静默执行，不会输出执行的指令
- 通常依赖的文件有很多，可以用`\`进行换行输入
- .PHONY:clean这句指令可以避免伪目标clean与目标重名
- rm指令前添加`-`，可以忽略有问题的文件，继续执行后续的清除指令

另外对于Makefile编译时的打印信息也可以进行静默编译，同C语言debug宏类似，使用ifeq进行判断是否需要进行静默编译：

```Makefile
# Allow for silent builds 静默编译
ifeq (,$(findstring s,$(MAKEFLAGS)))
XECHO = echo
else
XECHO = :
endif
```

### make参数

通过linux下执行`make --help`即可查看make参数的使用技巧。

### 赋值操作符

```makefile
var?=$(test)       # 当变量var未赋值(未定义)时则将$(test)赋值给var，若变量var已赋值则忽略$(test)
var+=$(test)       # 将$(test)的值添加在变量var结尾，中间用一个空格隔开，作用类似C语言中strcat
var:=$(test)       # 将表达式之前的$(test)的值赋给变量var，即使表达式之后$(test)值变化了也无法赋值，也就是说操作符`:=`只能向上搜索$(test)的值
var=$(test)        # 普通赋值，将最终的$(test)的值赋给变量var
```

### 函数使用

1. findstring(查找字符串函数)

```Makefile
# 示例：在字串<in>中查找<find>字符串，找到返回<find>，找不到则返回空字符串
$(findstring <find>,<in>)
# 应用：在变量$(MK_ARCH)中查找字符串"i386"、"i486"、"i586"、"i686"
$(findstring $(MK_ARCH), "i386" "i486" "i586" "i686")
```

2. filter(过滤函数)

```Makefile
# 示例：以<pattern>模式过滤<text>字符串中的单词，保留符合模式<pattern>的单词，可以有多个模式，返回符合模式<pattern>的字串
$(filter <pattern...>,<text>)
# 应用：在变量$(MAKEFLAGS)中过滤符合s%、-s%的单词
$(filter s% -s%,$(MAKEFLAGS))
```

3. firstword(首单词函数)

```Makefile
# 示例：取字符串<text>中的第一个单词，返回字符串<text>的第一个单词
$(firstword <text>)
# 应用：取变量$(MAKEFLAGS)中第一个单词，并与X进行组合
$(firstword x$(MAKEFLAGS))
```

4. origin(查询变量来源函数)

```Makefile
# 示例：<variable>是变量的名字，不是引用，所以不要添加$字符，通过函数返回值来确定此变量的来源
$(origin <variable>)
# 返回值：
undefined：表示此<variable>变量未定义
default：表示此<variable>变量是一个默认的定义，如‘CC’这个变量
environment：表示此<variable>变量是一个环境变量，并且执行Makefile时没有添加‘-e’参数(即未让环境变量覆盖Makefile变量)
file：表示此<variable>变量在Makefile中被定义
command line：表示此<variable>变量是被命令行定义的
override：表示此<variable>变量是被override指示符重新定义的
automatic：表示此<variable>变量是一个命令运行中的自动化变量，如$@、$<、$^等
# 应用：查询变量O的来源
$(origin O)
```

5. shell(执行shell命令函数)

```Makefile
# 示例：打开查看foo文件内容，与`cat foo`功能一致
$(shell cat foo)
# 应用：创建一个以变量$(KBUILD_OUTPUT)的文件夹并进入到文件夹内打印当前目录路径
$(shell mkdir -p $(KBUILD_OUTPUT) && cd $(KBUILD_OUTPUT) && /bin/pwd)
```


通配符：*、？、[]、%(规则通配符)

自动变量：$@(目标文件名) 、 $<(目标依赖名)、 $^(依赖的文件集合)、@?

gcc -M main.c执行后可以自动推导并列出编译main.c所需要的头文件

gcc -MM main.c执行后可以自动推导并输出非C标准库以外的头文件

vpath关键字：指定文件搜索目录
1. vpath %.h ../include  //指定在../include路径下查找所有.h的文件
2. vpath %.h             //清除符合%.h文件的../include搜索目录
3. vpath                 //清除所有设置好的文件搜索目录
