## 1. 前期准备

1.1 安装VScode软件，在扩展中搜索remote development插件、C/C++插件等进行安装

1.2 安装Git工具

1.3 安装openSSH：windos更新设置->更新与安全->开发者选项->选择开发人员模式->应用->可选功能->添加功能
- 安装openSSH服务器
- 安装openSSH客户端

1.4 配置openSSH环境变量：查看高级系统设置->环境变量->系统变量->Path
- 删除C:\Windows\System32\OpenSSH
- 增加C:\Program Files\Git\bin

## 2. Ubuntu配置

2.1 安装openSSH-server

```shell
sudo apt-get install openssh-server
```

2.2 备份原先的配置文件

```shell
sudo cp /etc/ssh/sshd_config /etc/ssh/sshd_config.backup
```

2.3 配置sshd config文件

```shell
sudo vim /etc/ssh/sshd_config  
PermitRootLogin yes            # 允许root用户通过ssh登录
PasswordAuthentication yes     # 密码登录
```

2.4 重新启动sshd服务

```shell
sudo service ssh --full-restart  # 重启sshd服务
sudo systemctl enable ssh        # 自动启动
```

2.5 查看服务器IP地址:ifconfig

注意：有时当输入ifconfig后只出现127.0.0.1本机地址，没有正常的IP地址，如下：

```shell
porter@ubuntu:~$ ifconfig
lo        Link encap:Local Loopback  
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:7156 errors:0 dropped:0 overruns:0 frame:0
          TX packets:7156 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:533469 (533.4 KB)  TX bytes:533469 (533.4 KB)
```

这种情况下表明ubuntu的虚拟网卡没有正常连接，需要手动重新启动连接。

## 3. VScode配置

3.1 配置ssh config
- 使用Ctrl+Shift+P
- 搜索Remote-SSH:Open Configuration File
- 选择C:\Users\用户组\.ssh\config
- 配置config:Host(可随意设置)、Hostname(服务器IP地址)、User(用户名)

3.2 连接服务器
- 使用Ctrl+Shift+P
- 搜索Remote-SSH:Open Configuration File
- Add New SSH Host
- 登录密码即可

## 4. 免密登录

复制id_rsa.pub全部内容，添加到到服务器的`~/.ssh/authorized_keys`文件中即可。

## 5. 全局索引设置

5.1 安装VScode插件：C/C++ GNU Global、C++ Intellisense

5.2 下载GNU Global win32版本可执行程序，[下载地址](http://adoxa.altervista.org/global/)，此系列程序用于执行gtags命令配置全局搜索，解压到C盘相应目录，如`C:\global\`

**注意：下载的版本号要与C/C++ GNU Global插件适配，在插件细节描述中会给出，当前要下载6.5以上版本**

5.3 将程序的bin目录添加进系统环境变量PATH中，如`C:\global\bin`

5.4 在VScode中，选择`文件->首选项->设置->用户->扩展->C/C++ GNU Global`，编辑Global Executable和Gtags Executable分别添加如下内容：

```javascript
"gnuGlobal.globalExecutable": "C:\\global\\bin\\global.exe",
"gnuGlobal.gtagsExecutable": "C:\\global\\bin\\gtags.exe"
```

5.5 打开VScode终端，输入gtags命令，可生成三个文件GPATH(定义的数据库)、GRTAGS(引用的数据库)、GTAGS(路径的数据库)

**补充：还有一种指定部分文件进行索引的方法，手动生成gtags.files，输入命令`find <dir1> -name "*.h" -print -o -name "*.c" -print > gtags.files`。**

## 6. VScode快捷键使用技巧

6.1 多行段落左移或右移
- 左移：选中内容后按`tab`实现自动右移
- 右移：选中内容后按`shift+tab`实现自动左移

6.2 常用快捷键
- `ctrl+shift+p`：打开全局命令框