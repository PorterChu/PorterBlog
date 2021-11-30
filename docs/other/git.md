## 1. Git介绍

Git是一个开源的分布式版本控制系统，用来进行代码跟踪和版本管理，尤其在敏捷开发中广泛使用。

## 2. Git配置

git config配置有三种级别，按照优先级以此为local(仓库)、global(用户)、system(系统)：

```shell
git config --local                                # 配置仓库级别
git config --global                               # 配置用户级别
  - git config --global user.name "姓名"
  - git config --global user.email "邮箱"
  - git config --global color.ui auto
git config --system                               # 配置系统级别
git config --list                                 # 查看全部配置
  - git config --local -l                         # 查看仓库配置
  - git config --global -l                        # 查看用户配置
  - git config --system -l                        # 查看系统配置
```

## 3. Git使用

### 3.1 git init--初始化仓库

### 3.2 git clone--获取远程仓库

```shell
git clone 仓库               # 克隆远程仓库
git clone -b 分支 仓库       # 克隆远程仓库指定分支
```

### 3.3 git status--查看仓库状态

### 3.4 git add--向暂存区添加文件

```shell
git add 文件名                # 添加单个文件
git add .                    # 添加全部文件
git add -A                   # 添加全部文件(A是All的简写)
```

### 3.5 git rm--移除文件

```shell
git rm -f 文件               # 从暂存区强制删除文件(f是force的简写)
git rm --cached 文件         # 从暂存区删除但在工作区保留
```

### 3.6 git mv--移动文件

### 3.7 git commit--保存仓库的历史记录

```shell
git commit -m "提交说明"      # 添加一行提交信息
git commit                   # 记述详细提交信息
git commit --amend           # 修改提交信息，补偿提交
```

### 3.8 git log--查看提交日志

```shell
git log -2                                   # 显示最近的两次提交
git log -p 文件                              # 显示文件的改动详情
git log --pretty             
  - git log --pretty=oneline                 # 将提交信息放在一行显示
  - git log --pretty=short                   # 将提交信息简短显示
  - git log --pretty=full                    # 将提交信息详细显示
  - git log --pretty=fuller                  # 将提交信息全部显示
  - git log --pretty=format
- git log --since=2019-1-1 --until=20191-30  # 查询2019/1/1-2019/1/30之间的提交记录
- git log --author="Porter"                  # 查询作者为Porter的提交记录
- git log --graph                            # 以图标形式查看分支
```

|选项|说明|
|:--:|:--:|
|%H|提交对象的完整哈希值|
|%h|提交对象的简短哈希值|
|-n|查询最近n条提交|
|--since,--after|查询指定时间之后的提交|
|--until,--before|查询指定时间之前的提交|
|--author|查询指定作者的提交|
|--committer|查询指定提交者的提交|

### 3.9 git reflog--查看当前仓库的操作日志
  
### 3.10 git diff--查看更改前后的差别

```shell
git diff                     # 查看工作区和暂存区的差别
git diff HEAD                # 查看暂存区与版本库的差别
```

*注意：`+`表示新添加的行，`-`表示被删除的行*

### 3.11 git pull

```shell
git pull --rebase
```

### 3.12 git rebase

### 3.13 git push

```shell
git push origin HEAD:refs/heads/master
```

### 3.14 git branch--显示分支列表

```shell
git branch -a                # 显示分支所有列表和当前所在分支
git branch -D 分支1          # 强制删除分支1
```

### 3.15 git checkout--创建、切换分支

```shell
git checkout -b 分支1 origin/分支2  # 创建本地分支1并切换到分支1，同时将本地分支1与远端分支2连接
git checkout -- *                  # 放弃所有修改，恢复原始内容
```

### 3.16 git reset--回溯历史版本

```shell
git reset --soft 哈希值      # 软重置，回退到原先的提交历史，保留对文件的修改
git reset --hard 哈希值      # 硬重置，回退到原先的提交历史，不保留对文件的修改
```

### 3.17 git merge--合并分支

```shell
git merge                       # 等同于git merge --ff，fast-forward方式合并，不会创建新的提交信息
git merge --no-ff 分支          # 关闭fast-forward(快进)方式，创建新的提交信息，便于分支回退和管理
git merge --squash              # 把多次分支的提交历史压缩为一次
```

### 3.18 git remote

```shell
git remote -v                          # 查看远端仓库地址
git remote set-url origin clone路径
```

### 3.19 git revert--

```shell
git revert 哈希值                      # 会在当前历史记录中创建新的提交信息还原之前的提交
```

### 3.20 git cherry-pick--

```shell
git cherry-pick 哈希值  
```

### 3.21 git format-patch--生成补丁(patch)

```shell
git format-patch -M master                 # 当前分支所有超出master的提交打包
git format-patch [哈希值]                  # 从哈希值之后所有的提交打包
git format-patch --root [哈希值]           # 从根到指定哈希值之间的所有提交打包
git format-patch [哈希值]..[哈希值]         # 两个哈希值之间所有的提交打包
git format-patch –n [哈希值]               # 包含当前在内的前n次提交打包
  - git format-patch -1 [哈希值]           # 单次提交打包
```

### 3.22 git am--应用补丁

```shell
git am [patch路径\diff路径]                # 应用patch
git am --abort                            # 终止应用patch
git am --continue                         # 继续应用patch
git am --skip                             # 跳过报错的patch
git am --resolved                         # 解决patch引起的冲突
```

### 3.23 git apply--应用补丁

```shell
git apply --check [patch路径\diff路径]     # 检查patch是否可以顺利利用
git apply [patch路径\diff路径]             # 应用patch
```

## 4. Git解疑

### 4.1 git gc清理仓库

git提交代码到远端仓库的时候，出现设备空间不足，此时需要清理仓库，使用`git gc`即可，然后再次提交代码就行了。

### 4.2 git pull\push报错

当`git pull`或`git push`时出现错误，如下：

```bash
ERROR: Repository not found.
fatal: Could not read from remote repository.
Please make sure you have the correct access right and the repository exists.
```

此问题通过git bash分步骤来排查
- 第一步：输入`git remote -v`来检查远端仓库是否是想要push的仓库
- 第二步：输入`ssh -T git@github.com`来检查是否连接Github成功
- 第三步：进入根目录`cd ~/`来确认公钥和私钥存放位置是否被修改，通常SSH公钥和私钥路径会被切换到*/SPB_Data下面，此时copy一份公钥和私钥到对应路径即可