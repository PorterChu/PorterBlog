1. 关闭进程、删除lock文件

当我们使用`sudo apt install openssh-server`命令去安装openssh-server时出现如下错误：

```c
E: Could not get lock /var/lib/dpkg/lock-frontend - open (11: Resource temporarily unavailable)
E: Unable to acquire the dpkg frontend lock (/var/lib/dpkg/lock-frontend), is another process using it?
```

此时表示apt进程正在运行，需要先关闭apt进程，方法如下：

```c
ps -A | grep apt
78471 ?        00:00:00 apt.systemd.dai  //运行的78471进程
78486 ?        00:00:00 apt.systemd.dai  //运行的78486进程
sudo kill 78471 78486
```

然后再次运行`sudo apt install openssh-server`命令，如果成功则OK，若不成功则实行如下方法：

```c
sudo rm /var/lib/dpkg/lock-frontend /var/lib/dpkg/lock
```

最后再一次运行`sudo apt install openssh-server`进行安装更新。

2. 'main'未引用错误

当一个.c文件使用`gcc`命令进行编译时，出现下方的错误：
```c
/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64/crt1.o：在函数‘_start’中：
/glibc/glibc-2.15/csu/../sysdeps/x86_64/elf/start.S:109：对‘main’未定义的引用
```

这是因为使用`gcc`会直接进行代码编译(预处理、编译器、汇编器)并且尝试进行链接成可执行文件，而文件中没有main函数的定义，所以才会报错，解决此类报错的方法即使用`gcc -c`来替代，`-c`参数表明将.c文件编译成目标文件不进行链接，这样就不会进行报错了。

```c
gcc –E   //预处理，生成.i文件
gcc –S   //编译，生成.s文件
gcc -c   //汇编，生成.o文件
```
