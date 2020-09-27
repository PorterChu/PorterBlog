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