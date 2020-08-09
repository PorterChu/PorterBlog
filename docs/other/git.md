## Git介绍

Git是一个开源的分布式版本控制系统，在敏捷开发中广泛使用进行代码跟踪和版本管理。

## Git配置

git config --global user.name "姓名"
git config --global user.email "邮箱"
git config --global color.ui auto

## Git使用

### git init--初始化仓库

git init是对创建的本地仓库进行初始化，初始化完成后会生成`.git`目录，这个目录被称为“附属于该仓库的工作树”，包含了git操作所需要的所有配置，如果后期想将文件恢复到原先的状态，就可以从仓库中调取之前的哈希值，目录下的index是一个二进制文件，即为仓库的暂存区。

### git clone--获取远程仓库

### git status--查看仓库状态

### git add--向暂存区添加文件

- git add 文件名                //添加单个文件
- git add .                    //添加全部文件
- git add -A(A是All的缩写)      //添加全部文件

### git commit--保存仓库的历史记录
- git commit -m "添加提交说明"  //添加一行提交信息
- git commit                   //记述详细提交信息
- git commit --amend           //修改提交信息，补偿提交

### git log--查看提交日志

- git log -p 文件目录           //显示文件的改动详情
- git log --graph              //以图标形式查看分支
  
### git reflog--查看当前仓库的操作日志
  
### git diff--查看更改前后的差别

- git diff                      //查看工作区和暂存区的差别
- git diff HEAD                 //查看暂存区与版本库的差别

*注意：`+`表示新添加的行，`-`表示被删除的行*

### git pull

### git rebase

### git push

- git push origin HEAD:refs/heads/master

### git branch--显示分支列表

- git branch -a                //显示分支所有列表和当前所在分支
- git branch -D

### git checkout--创建、切换分支

- git checkout -b 分支名

### git reset--回溯历史版本

- git reset -- soft 哈希值(4位以上)
- git reset -- hard 哈希值(4位以上)

### git merge--合并分支

- git merge --no-ff 分支名

### git remote

- git remote -v
- git remote set-url origin clone路径