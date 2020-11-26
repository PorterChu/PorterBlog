## 1. Git介绍

Git是一个开源的分布式版本控制系统，用来进行代码跟踪和版本管理，尤其在敏捷开发中广泛使用。

## 2. Git配置

- git config --global user.name "姓名"
- git config --global user.email "邮箱"
- git config --global color.ui auto
- git config --list                       //查看配置

## 3. Git使用

### 3.1 git init--初始化仓库

### 3.2 git clone--获取远程仓库

### 3.3 git status--查看仓库状态

### 3.4 git add--向暂存区添加文件

- git add 文件名                //添加单个文件
- git add .                    //添加全部文件
- git add -A                   //添加全部文件(A是All的简写)

### 3.5 git rm--移除文件

- git rm -f 文件               //从暂存区强制删除文件(f是force的简写)
- git rm --cached 文件         //从暂存区删除但在工作区保留

### 3.6 git mv--移动文件


### 3.7 git commit--保存仓库的历史记录

- git commit -m "提交说明"      //添加一行提交信息
- git commit                    //记述详细提交信息
- git commit --amend            //修改提交信息，补偿提交

### 3.8 git log--查看提交日志

- git log -2                   //显示最近的两次提交
- git log -p 文件              //显示文件的改动详情
- git log --pretty             
  - git log --pretty=oneline   //将提交信息放在一行显示
  - git log --pretty=short     //将提交信息简短显示
  - git log --pretty=full      //将提交信息详细显示
  - git log --pretty=fuller    //将提交信息全部显示
  - git log --pretty=format
  
|选项|说明|
|:--:|:--:|
|%H|提交对象的完整哈希值|
|%h|提交对象的简短哈希值|

- git log --graph              //以图标形式查看分支
  
### 3.9 git reflog--查看当前仓库的操作日志
  
### 3.10 git diff--查看更改前后的差别

- git diff                     //查看工作区和暂存区的差别
- git diff HEAD                //查看暂存区与版本库的差别

*注意：`+`表示新添加的行，`-`表示被删除的行*

### 3.11 git pull

- git pull --rebase

### 3.12 git rebase

### 3.13 git push

- git push origin HEAD:refs/heads/master

### 3.14 git branch--显示分支列表

- git branch -a                //显示分支所有列表和当前所在分支
- git branch -D 分支1          //强制删除分支1

### 3.15 git checkout--创建、切换分支

- git checkout -b 分支1 origin/分支2  //创建本地分支1并切换到分支1，同时将本地分支1与远端分支2连接

### 3.16 git reset--回溯历史版本

- git reset --soft 哈希值(4位以上)  //软重置，回退到原先的提交历史，保留对文件的修改
- git reset --hard 哈希值(4位以上)  //硬重置，回退到原先的提交历史，不保留对文件的修改

### 3.17 git merge--合并分支

- git merge                       //等同于git merge --ff，fast-forward方式合并，不会创建新的提交信息
- git merge --no-ff 分支          //关闭fast-forward(快进)方式，创建新的提交信息，便于分支回退和管理
- git merge --squash              //把多次分支的提交历史压缩为一次

### 3.18 git remote

- git remote -v                   //查看远端地址
- git remote set-url origin clone路径

### 3.19 git revert--

- git revert 哈希值(4位以上)      //会在当前历史记录中创建新的提交信息还原之前的提交

### 3.20 git cherry-pick--

- git cherry-pick 哈希值(4位以上) 

## 4. Git解疑

### 4.1 git gc清理仓库

git提交代码到远端仓库的时候，出现设备空间不足，此时需要清理仓库，使用`git gc`即可，然后再次提交代码就行了。