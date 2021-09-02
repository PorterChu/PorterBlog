## shell介绍

shell种类较多，最常用也是大多数linux系统默认的是bash shell，即Bourne Again Shell(/bin/bash)，但通常并不与Bourne Shell(/bin/sh)区分开来，可以当作同一个。

shell编程与其他脚本编程一样，只需能编写代码的文本编辑器和解释执行的脚本解释器就行了。首先运行shell程序会区分在新进程中运行还是在当前进程中运行，可以通过进程ID(PID)来进行区分。

### 1. 在新进程中运行shell程序

#### 1.1 作为可执行程序

```shell
touch test.sh
chmod +x ./test.sh
./test.sh
```

这边`chmod +x ./test.sh`表示给当前目录下的test.sh文件增加<font color=pink>执行权限</font>，这如何理解呢？在linux下你是否留意过不同文件有不同的颜色？

区分如下：

- 蓝色：目录(文件夹)
- 绿色：可执行文件或可执行程序
- 红色：压缩文件或包文件
- 浅蓝色：链接文件，软链接或硬链接
- 白色：普通文件

当用`touch`命令新创建一个test.sh时，这个文件颜色是白色的，表示为普通文件，但只有可执行文件或可执行程序才可直接执行，直接执行普通文件会报错，这时就需要`chmod +x`出场了，`chmod +x ./test.sh`这句话的意思是将当前目录下的test.sh的普通文件转换成可执行文件。

另外还有个知识点需要注意，通常会在脚本中看到`chmod a+x`，这里的`u`、`g`或者`a`等字符表示授予谁可以执行该文件：

- u：user，表示用户
- g：group，表示用户组
- o：other，表示其他
- a：all，表示所有

*当然这边授予文件全部执行权限除了使用`chmod +x`外，也可以使用`chmod 777`，chmod后面三位数字分别代表user、group、others的权限，而文件可读(r)、可写(w)、可执行(x)的权限分别用数字4、2、1表示，数字7是权限4、2、1的和，777即代表user、group、others均具有可读(r)、可写(w)、可执行(x)的权限，chmod 777表示释放了全部权限。*

#### 1.2 作为解释器参数

```shell
/bin/sh test.sh   # 这种方式执行脚本则不需要在脚本中指定解释器信息(下文再阐述)
```

甚至更为简单的用bash来进行执行：

```shell
bash test.sh
```

区别在于，第一种方式已经指出了bash解释器的绝对路径，可以直接运行；第二种方式用bash解释器来运行的时候，会多一个查找路径的动作。

### 2. 在当前进程中运行shell程序

这边要引入一个新的命令`source`，这个命令会读取脚本文件中的代码，并强制执行所有语句，不考虑文件的执行权限，也就是说不需要使用`chmod +x`来更改文件的执行权限了。

```shell
source test.sh
```

也可以简写成：*注意：这边用`.`来执行时，文件名前有个空格*

```shell
. test.sh
```

### 3. 如何确定新进程和当前进程

上面提到用进程ID(PID)可以来确定是新进程还是当前进程，用`echo $$`命令可输出PID。

```shell
porter@ubuntu:~/windows_share$ touch test.sh   # 创建脚本文件
porter@ubuntu:~/windows_share$ vim test.sh     # 写入`echo $$`用来输出PID
porter@ubuntu:~/windows_share$ . test.sh       # 在当前进程中执行脚本
18100                                          # 当前进程ID
porter@ubuntu:~/windows_share$ . test.sh
18100                                          # 当前进程ID
porter@ubuntu:~/windows_share$ bash test.sh    # 在新进程中执行脚本
4802                                           # 新进程ID
porter@ubuntu:~/windows_share$ bash test.sh
5035                                           # 新进程ID
porter@ubuntu:~/windows_share$ bash test.sh
5038                                           # 新进程ID
```

从上面注释中看到，在当前进程中执行脚本，无论执行多少次PID都不会变，而在新进程中执行脚本，每次输出的PID都是不同的。同时，在当前进程中使用`. filename`最方便，在新进程中使用`bash filename`最方便，而且不需要更改文件的执行权限。

## shell编程

### 变量定义与初始化

```shell
string="hello"  # 注意：'='后面不能有空格，语句结尾不能添加分号
```

### 变量赋值

### 变量引用

变量引用必须使用'$'符号，类似与C语言中的'@'解引用
注意：$引用的变量未定义的话，执行输出的值就为空

引用$
单引号''
双引号""
反引号``

1. if判断语句

```shell
if []; then
        xxx
else
        xxx
fi
```

- if判断文件是否存在(-f)
- if判断目录是否存在(-d)
- if判断字符串是否相等("str1" = "str2")
- if判断字符串是否为空(-z)
- if判断数字是否相等(-eq)、不等于(-ne)、大于(-qt)、小于(-lt)、大于等于(-ge)、小于等于(-le)

2. while do循环语句

```shell
while []
do
        xxx
done
```

shell编程实现i++的几种方法：
- i=$(($i + 1))
- i=$[$i+1]
- let i++
- let i+=1
- i=`expr $i + 1`

3. case语句

```shell
case xxx in
1) xxx ;;
2) xxx ;;
esac 
```

注意：shell编程中case语句执行完是默认执行break的，如果与while do循环语句结合的case语句中有break，那这个break是用来跳出while do循环的。

$@:目标 $#:传参个数 $0：执行指令 $1：第一个有效传参 有效传参有几个，$#就为几

shift可以修改传参指令，作用类同于左移运算符

echo xxx > a.c：创建新文件a.c并输入内容xxx
echo xxx >> a.c 在文件a.c末尾中追加内容xxx

## shell使用

1. 通配符

```shell
?              # 通配一个字符
*              # 通配任意字符
[ai]           # 通配a和i字符
[a - i]        # 通配a到i之间多字符
[!a]           # 通配非a字符
```

2. 查看信息

```shell
ls                             # 查看文件
  - ls -l                      # 查看文件详细信息，不包括隐藏文件
  - ls -a                      # 查看所有文件，包括隐藏文件
df                             # 查看文件系统硬盘使用情况
  - df -a                      # 显示所有文件系统，包括/proc、/sysfs等系统
  - df -h                      # 使用习惯单位显示
du                             # 统计目录或文件所占磁盘空间大小
  - du -h xxxx                 # 使用习惯单位查看文件xxxx的占用大小
  - du -sh
file xxx                       # 查看文件xxx类型
uname -m                       # 查看CPU架构平台
uname -r                       # 查看内核版本
history                        # 查看历史命令
top                            # 查看CPU负载信息
  - q                          # 退出top
tree -L 4 -d                   # 查看文件树，-L表示显示目录深度4，-d表示只显示目录
cat xxx                        # 查看全部文本内容
  - cat -n xxx                 # 添加行号查看文本内容
  - cat -b xxx                 # 仅添加文本行号查看文本内容
  - cat /etc/issue             # 查看服务器版本
more xxx                       # 以页面形式查看文本内容
less xxx
tail xxx                       # 默认仅显示文本最后10行
head xxx                       # 默认仅显示文本前10行
dmesg                          # 用于检查和控制内核的ring buff的消息，路径/var/log/dmesg
  - dmesg -c                   # 显示信息后清楚ring buff中的内容
  - dmesg -s                   # 设置缓冲区大小，预设值8196
  - dmesg -n                   # 设置记录信息的层级
```

3. 压缩和解压

```shell
unzip filename.zip                           # 解压zip格式文件
tar czvf filename.tar.gz file1 file2 ...     # 多个文件压缩成gz格式
tar -zxvf filename.tar.gz                    # 解压gz格式文件
tar -jxvf filename.tar.xz                    # 解压xz格式文件
tar -zxvf filename.tar.Z                     # 解压Z格式文件
tar --help                                   # 查看tar参数使用规则
```

4. 建立软链接

```shell
ln -s 源文件目录 目标文件目录    # 将源文件目录映射到目标文件目录
```

5. 查找文件或内容

```shell
find 路径 -name "xxx"             # 路径表示想要在哪个目录下找到xxx文件
grep -nR "xxx"                    # 查找xxx内容
```

6. 修改权限

```shell
chmod 777 *                     # 修改文件权限
chown porter:porter xxx         # 更改xxx文件属主和属组为porter
```

7. 查看和终止进程

```shell
ps                             # 查看进程信息
  - ps -ef                     # 查看所有进程并输出完整格式
  - ps -ef | grep -i xxx       # 查看xxx(不区分大小写)进程信息
  - ps -l
kill PID                        # 终止PID编号的进程
  - kill -s PID                 # 强制终止PID编号的进程
killall UID                     # 终止UID编号的所有进程
```

8. 内核模块操作

```shell
insmod *.ko                     # install module，载入内核模块
rmmod *                         # remove module，卸载内核模块，注意不需要添加.ko后缀
lsmod *.ko                      # list module，查看内核模块
modinfo *.ko                    # module information，打印内核模块信息
modprobe
  - modprobe -l                 # 查看所有模块
  - modprobe *.ko               # 载入内核模块
  - modprobe -r *.ko            # 卸载内核模块
```

9. patch生成和应用

```shell
diff
  - diff -u original.c new.c > patch-to-file.patch # 为某一文件创建patch
  - diff -rupN original/ new/ > patch-to-dir.patch # 为源树创建patch
git format-patch -1 xxx(哈希值)
patch -pn < xxx.diff                               # 忽略n层目录打入patch
  - patch -p1 < xxx.diff                           # 忽略第一层目录打入patch(需要把patch放在路径中上一层目录)
  - patch -p1 -R < xxx.patch                       # 删除patch
git am xxx.patch
```

10. 挂载和卸载

```shell
mount -o rw,remount /  # 以读写和重新挂载的方式对当前路径进行挂载操作
```