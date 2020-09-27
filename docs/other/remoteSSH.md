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

```c
sudo apt-get install openssh-server
```

2.2 备份原先的配置文件

```c
sudo cp /etc/ssh/sshd_config /etc/ssh/sshd_config.backup
```

2.3 配置sshd config文件

```c
PermitRootLogin yes            //允许被登录
PasswordAuthentication yes     //密码登录
```

2.4 重新启动sshd服务

```c
sudo service ssh --full-restart  //重启sshd服务
sudo systemctl enable ssh        //自动启动
```

2.5 查看服务器IP地址:ifconfig

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