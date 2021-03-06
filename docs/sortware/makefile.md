## Makefile介绍

Makefile是用来编译大型软件工程的，Makefile对语法格式非常在意，很多语法格式和shell编程类似，详细的Makefile写作细节可参考陈皓大神的《跟我一起写Makefile》。

## Makefile提炼

Makefile写好配置后，在linux下只需要执行make就可自动进行编译，linux的make指令会在当前目录下寻找Makefile或makefile文件进行编译。

### 通用格式

```Makefile
目标：依赖
    @指令1；指令2；指令3
.PHONY:clean
clean:
    -rm 文件
```

注意：
- 执行的gcc指令之前必须是1个tab键，不能以4个空格键代替，否则会报错
- 在gcc指令之前添加`@`后，Makefile就会静默执行，不会输出执行的指令
- 如果多条指令需要执行且上条指令结果应用在下条指令时，需要把多条指令写在同一行并用分号分隔
- 通常单行的文件有很多时，可以用换行符`\`进行换行输入
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

### 文件引用

使用include关键字在当前Makefile中引用其他文件，作用类似于C语言中的#include，语法如下：

```Makefile
-include <filename>
```

注意：
- include前添加的`-`，表示在引用文件的过程中无需理会错误，继续执行
- include前可以有空字符，但不能是tab键
- include会在当前路径下寻找，同时也会在`/usr/local/bin或/usr/include`下寻找，若执行make时指定了`-I或--include-dir`，也会到指定路径下寻找文件

```Makefile
print: *.c
lpr -p $?            #  $?列出比目标文件(print)更新的所有依赖文件，并由lpr命令提交给打印机
touch print
```

### make参数

通过linux下执行`make --help`即可查看make参数的使用技巧：

```Makefile
make -n\--just-print  # 只显示命令，不会执行命令
make -s\--slient      # 全面禁止命令的显示
make -e               # 总控Makefile的变量覆盖下层Makefile中所定义的变量
make -C               # 指定执行目录
make -w               # 打印当前目录
```

### 赋值操作符

```makefile
var?=$(test)       # 当变量var未赋值(未定义)时则将$(test)赋值给var，若变量var已赋值则忽略$(test)
var+=$(test)       # 字符串追加，将$(test)的值添加在变量var结尾，中间用一个空格隔开，作用类似C语言中strcat
var:=$(test)       # 将表达式之前的$(test)的值赋给变量var，即使表达式之后$(test)值变化了也无法赋值 \
                     也就是说操作符 `:=` 只能向上搜索$(test)的值
var=$(test)        # 普通赋值，将最终的$(test)的值赋给变量var
$(var:a=b)         # 字符串替换，将变量var中以a结尾的字符串a替换成b
```

### 函数使用

- 字符串处理函数

1. findstring(查找字符串函数)

```Makefile
# 示例：在字串<in>中查找<find>字符串，找到返回<find>，找不到则返回空字符串
$(findstring <find>,<in>)
# 应用：在变量$(MK_ARCH)中查找字符串"i386"、"i486"、"i586"、"i686"
$(findstring $(MK_ARCH), "i386" "i486" "i586" "i686")
```

2. filter(过滤函数)、filter-out(反过滤函数)

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

4. wildcard(扩展通配符)

在Makefile规则中，通配符会被自动展开，但在变量的定义和函数引用时，通配符将失效，这种情况下如果需要通配符有效，就需要使用函数'wildcard'。规则是展开已经存在的、使用空格分开、匹配此模式的所有文件列表，如果不存在任何符合此模式的文件，则函数会忽略模式字符返回空。

```Makefile
objects:=$(wildcard *.c)  # 展开所有的.c文件
```

5. subst(字符串替换函数)、patsubst(通配符字符串替换函数)

```Makefile
# 示例：将字符串<text>中的<from>替换成<to>
$(subst <from>,<to>,<text>)
# 应用：将字符串variable1中的1更换为2
$(subst 1,2,variable1)   
```

6. VPATH(大写，变量)和vpath(小写，关键字)：指定文件搜索目录

```Makefile
VPATH=src:../headers     # 在'src'和'../headers'两个目录中查找文件，目录之间用冒号':'分隔
vpath %.h ../include     # 指定在../include路径下查找所有.h的文件
vpath %.h                # 清除符合%.h文件的搜索目录
vpath                    # 清除所有设置好的文件搜索目录
```

7.  strip(去空格函数)

```Makefile
# 应用：去掉<string>字串中开头和结尾的空字符,并将中间的多个连续空字符(如果有的话)合并为一个空字符
$(strip <string>)        
```

8. sort(排序函数)

```Makefile
# 示例：给字符串<list>中的单词排序(升序)
$(sort <list>)
# 应用：字符串排序为bar foo lose
$(sort foo bar lose)
```

9. word(取单词函数)

```Makefile
# 示例：取字符串<text>中第<n>个单词，从1开始
$(word <n>,<text>)
# 应用：取出的单词为bar
$(word 2, foo bar baz)
```

10. wordlist(取单词串函数)

```Makefile
# 示例：从字符串<text>中取从<s>开始到<e>的单词串，注意：<s>和<e>是一个数字
$(wordlist <s>,<e>,<text>)
# 应用：去除的单词串为bar baz
$(wordlist 2, 3, foo bar baz) 
```

11. words(单词个数统计函数)

```Makefile
# 示例：统计<text>中字符串中的单词个数
$(words <text>)
# 应用：统计单词个数为3
$(words, foo bar baz)             
```

- 文件名操作函数

1. dir(取目录函数)

```Makefile
# 示例：从文件名序列<names>中取出目录部分，目录部分是指最后一个反斜杠('/')之前的部分，如果没有反斜杠，那么返回'./'
$(dir <names...>)
# 应用：取出的目录为‘src/ ./’
$(dir src/foo.c hacks)
```

2. notdir(去除路径函数)

```Makefile
# 示例:从文件名序列<names>中取出非目录部分。非目录部分是指最后一个反斜杠('/')之后的部分
$(notdir <names...>)
# 应用：去除路径执行结果为foo.c hacks
$(notdir src/foo.c hacks)
```

- origin(查询变量来源函数)

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

- shell(执行shell命令函数)

```Makefile
# 示例：打开查看foo文件内容，与`cat foo`功能一致
$(shell cat foo)
# 应用：创建一个以变量$(KBUILD_OUTPUT)的文件夹并进入到文件夹内打印当前目录路径
$(shell mkdir -p $(KBUILD_OUTPUT) && cd $(KBUILD_OUTPUT) && /bin/pwd)
```

### 书写规则

1. 通配符

*、？、[]、%(规则通配符)

2. 自动变量

```Makefile
$@(目标文件集)
$<(依赖目标中第一个目标名字或符合模式的依赖目标集)
$^(依赖目标集)
$?(所有比目标新的依赖目标集)
$+(依赖目标集，与$^类似，但不去除重复的依赖目标)
$*(识别模式匹配前的字符串)
```
3. 依赖推导

```Makefile
gcc -M main.c      # 执行后可以自动推导并列出编译main.c所需要的头文件
gcc -MM main.c     # 执行后可以自动推导并输出非C标准库以外的头文件
```

