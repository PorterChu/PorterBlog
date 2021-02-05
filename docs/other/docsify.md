## Docsify介绍

&emsp;&emsp;docsify是一款神奇的文档站点生成器，可以直接加载和解析[Markdown](/docs/other/markdown.md)文件并生成本地网站`http://losthost:3000`供浏览观看，当然如果不满足于只能自己本地观看，想要生成固定的网址供他人浏览，也可以搭载Github工具通过git生成属于自己的个人网站，下面就来看看如何配置docsify站点。

## Docsify配置

&emsp;&emsp;在配置docsify之前需要npm环境的支持，而npm环境的配置又离不开Node.js软件，所以第一步就是要下载系统对应的软件版本进行安装，可以通过点击[Node.js](http://nodejs.cn/download/)链接到下载网页，也可以通过Github获取相应的软件包文件，安装过程就不阐述了。

安装完成后，通过`win+R`运行windows DOS命令台，输入`node -v`和`npm -v`来检查当前版本：

```shell
C:\Users\86187>node -v                       # 查看node.js版本
v14.6.0
C:\Users\86187>npm -v                        # 查看npm版本
6.14.6
```

因为npm服务器在境外，直接使用境外服务器配置docsify会非常慢，接下来就需要设置国内镜像来为后续的配置做准备，国内镜像源有很多，我们这边使用淘宝的镜像：

```shell
npm config set registry=https://registry.npm.taobao.org  # 设定淘宝镜像源
```

可以通过`npm config list`来确认镜像源是否配置成功：

```shell
C:\Users\86187>npm config list
; cli configs
metrics-registry = "https://registry.npm.taobao.org/"

; userconfig C:\Users\86187\.npmrc
registry = "https://registry.npm.taobao.org/"
```

通常我们个人上网无需使用代理设置，所以不需要设定代理服务器路径，但某些公司内有外网权限管控，此时就需要额外设置代理服务器路径(打开google浏览器查看设置里面的代理地址和端口)：

```shell
npm config set proxy http://address:port        # 个人家庭上网无需设置
npm config set https-proxy http://address:port  # 个人家庭上网无需设置
```

接下来就要安装docsify工具了：

```shell
npm install docsify-cli -g                      # 安装docsify
```

输入`docsify -v`检查docsify版本来确认docsify安装成功：

```shell
C:\Users\86187>docsify -v                       # 查看docsify版本
docsify-cli version:
  4.4.1
```

搜索`Windows PowerShell`并以管理员模式运行：

```shell
set-ExecutionPolicy RemoteSigned                # 执行此命令需要按`Y`确定
get-ExecutionPolicy 
```

执行完以上步骤后docsify站点就算配置成功了，快要大功告成了，接下来在硬盘随便建一个文件夹并用VScode打开，在终端下进行docsify初始化：

```shell
docsify init                                    # 初始化docsify
```

初始化成功后当前文件目录内便会出现index.html、README.md和.nojekyll三个文件，最后我们来借助这几个文件执行如下命令触发本地网页站点，并可以点击链接查看网页啦：

```shell
docsify serve ./                                # 配置本地网站站点
```

如果git库原先有已经写好的文档，则不需要初始化本地仓库，直接进入到对应的目录直接进行配置就行啦，如下示例：

```shell
docsify serve docs                              # 配置本地网站站点，通常为：http://localhost:3000
```

