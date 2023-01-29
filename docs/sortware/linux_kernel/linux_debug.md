## 1. 关闭进程、删除lock文件

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

## 2. 'main'未引用错误

当一个.c文件使用`gcc`命令进行编译时，出现下方的错误：
```c
/usr/lib/gcc/x86_64-redhat-linux/4.8.5/../../../../lib64/crt1.o：在函数‘_start’中：
/glibc/glibc-2.15/csu/../sysdeps/x86_64/elf/start.S:109：对‘main’未定义的引用
```

这是因为使用`gcc`会直接进行代码编译(预处理、编译器、汇编器)并且尝试进行链接成可执行文件，而文件中没有main函数的定义，所以才会报错，解决此类报错的方法即使用`gcc -c`来替代，`-c`参数表明将.c文件编译成目标文件不进行链接，这样就不会进行报错了。

gcc编译过程分步解析如下：

```c
gcc 文件名         //直接编译，生成a.out可执行文件
gcc 文件名 -o xxx  //也是直接编译，生成名为xxx的可执行文件
gcc –E 文件名      //预处理过程，生成.i文件
gcc –S 文件名      //编译过程，生成.s文件
gcc -c 文件名      //汇编过程，生成.o文件
```

## 3. VMware与Device/Credential Guard不兼容

打开VMware时会遇到与Device/Credential Guard不兼容导致VMware无法正常启动Ubuntu系统，此时禁用Device/Credential Guard后便可以正常运行VMware Workstation，可能原因是windows系统的Hyper-V不兼容导致，debug步骤如下：

- 第一步：按`win+R`键弹出运行框，输入`gpedit.msc`打开`本地组策略编辑器`，选择`本地计算机策略->计算机配置->管理模块->系统->Device Guard->打开基于虚拟化的安全`，选择`禁用`，然后点击`应用`和`确定`按钮；
- 第二步：按`win+R`键弹出运行框，输入`services.msc`后打开`服务(本地)->HV主机服务`，选择启动类型为`禁用`，然后点击`应用`和`确定`按钮；
- 第三步：选择`控制面板->程序与功能->启用或关闭Windows功能`，取消`Hyper-V`的勾选，点击`确定`按钮；
- 第四步：按`win+X`键以管理员身份运营Windows PowerShell，输入`bcdedit /set hypervisorlaunchtype off`，如果后期要重启开启Hyper，可以同样在PowerShell下输入`bcdedit /set hypervisorlaunchtype auto`;
- 第五步：重启电脑后，运行VMware Workstation就可正常工作了。

## 4. dos与unix换行符不兼容

碰到在bash中报`syntax error near unexpected token $'\r''`，或者在Kconfig中报`syntax error`错误时，可以通过`vi -b`的命令查看对应的文件，发现文件中每一行结尾多了`^M`的换行符，问题出在此处，可以使用如果命令进行转换：

```shell
dos2unix file   # 转换windows换行符为unix换行符
```
