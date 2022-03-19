## 1. Repo介绍

repo是谷歌开发的一款工具，repo并非用来取代git，而是用来配合git工具更好的让开发者管理Android代码。

repo工具实际上分为两部分，第一部分是我们需要安装的repo工具启动器，第二部分则包含在源代码中(无需我们额外操作)，第一部分安装后可以与第二部分进行通信以此来管理源代码。

## 2. Repo安装

```shell
mkdir ~/bin
PATH=~/bin:$PATH
curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo > ~/bin/repo
chmod a+x ~/bin/repo
export REPO_URL='https://mirrors.tuna.tsinghua.edu.cn/git/git-repo' # 或者在~/bin/repo中手动将repo改为清华镜像
```

## 3. Repo使用

### 3.1 repo help--查看所有的repo指令

```shell
repo help {init|...}    # 查看repo init的详细传参使用方法，效果等同于repo {init|...} help
```

### 3.2 repo init--在当前目录安装repo工具，创建一个`.repo`目录，其中包含repo源代码和Android清单文件的git代码库

```shell
repo init -u            # 指定从中检索清单代码库的网址，通常默认是：https://android.googlesource.com/platform/manifest
repo init -m            # 选择代码库中的清单文件，若未选择，则默认为default.xml
repo init -b            # 指定分支版本，即特定的manifest分支
```

### 3.3 repo sync--从远端库上下载代码最新修改并更新本地文件夹

```shell
repo sync               # 同步所有项目的文件
```

- 运行`repo sync`注意：
  - 若目标项目第一次同步：运行结果等同于`git clone`
  - 若目标项目非第一次同步：运行结果等同于`git remote update`和`git rebase origin/分支`
  - 若同步导致合并冲突：使用`git rebase --continue`解决冲突

### 3.4 repo upload--上传全部项目中代码的修改

```shell
repo upload --current-branch  # 只上传当前分支中代码的修改部分
```

### 3.5 repo diff--显示当前提交与工作树之间的更改信息


### 3.6 repo download--从库上下载更改的文件，并放在项目的本地工作目录中使用

```shell
repo download platform/build xxxx   # 下载文件xxxx到platform/build目录中
```

### 3.7 repo forall--在项目中运行shell命令


### 3.8 repo prune--删减(删除)已合并的主题


### 3.9 repo start--创建一个新的分支进行开发


### 3.10 repo status--将工作树与临时区域(索引)以及当前分支(HEAD)上的最近一次提交进行比较，显示项目中每个文件的状态信息