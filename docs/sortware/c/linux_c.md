## 1. getopt函数

### 1.1 介绍

getopt函数定义如下：

```c
int getopt(int argc,char * const argv[ ],const char * optstring);
```

getopt函数是用来处理命令行选项参数的，参数结构上和main函数很相似，int argc和char *const argv[]两个参数同main一致，表示传入的参数个数和参数字符串数组，不同之处在于，getopt函数比main多了一个参数：const char *optstring，从名字可以看出是一个字符串，叫选项字符串。

在详细介绍getopt函数功能之前，还要介绍几个同样附属在unistd.h下的API，先初步介绍下：

```c
extern char* optarg;  /*用来保存检索到的选项的参数*/
extern int optind;    /*用来记录下一次检索的位置*/
extern int opterr;    /*用来表示是否错误输出到stderr，为0时不输出*/
extern int optopt;    /*用来表示不在选项字符串规定里的选项符号*/
```

这边有两个概念：

- 选项

如下，在linux下执行删除文件夹的命令：

```c
mkdir temp             /*创建文件夹*/
rm -rf temp            /*删除文件夹*/
```

rm是删除指令，这个不用多说，这边的-rf就是选项命令行，-r表示递归删除，-f表示不提醒立刻删除，连写在一起就是-rf。

- 选项字符串

有了上面的铺垫，选项字符串就很好理解了，表示getopt函数所用到的所有选项符号的集合，不在选项字符串内的符号就可以通过optopt表示。

选项字符串中可以连写，如`a:bcd::ef:`，对应的选项命令行-a，-b，-c，-d，-e，-f，这边冒号代表是否需要在选项命令后面加参数，<font color=blue>单冒号'：'表示需要参数（没有参数会报错），这个参数可以和选项连在一起写，也可以用空格隔开，</font>比如-a123 和-a 123（中间有空格），这两组都表示123是-a的参数；<font color=blue>双冒号'::'表示选项的参数是可选的，可以带参数，也可以不带参数，如果带参数，参数与选项之间不能有空格。</font>

### 1.2 返回值

如果optind在optstring中成功检索，则返回选项字符；如果所有命令行选项都解析完毕则返回‘-1’；如果检索到选项字符不在optstring中则返回字符'?'如果遇到丢失参数，那么返回值依赖于optstring中第一个字符，如果第一个字符是':'则返回':'，否则返回'?'并提示出错误信息。

### 1.3 示例说明

示例代码一:

```c
printf("optind:%d,opterr：%d\n",optind,opterr);
printf("--------------------------\n");
while ((ch = getopt(argc, argv, "ab:c:de::")) != -1)
{
    printf("optind: %d\n", optind);
    switch (ch)
    {
  case 'a':
   printf("HAVE option: -a\n");
   break;
  case 'b':
   printf("HAVE option: -b\n");
    printf("The argument of -b is %s\n", optarg);
   break;
  case 'c':
   printf("HAVE option: -c\n");
   printf("The argument of -c is %s\n", optarg);
   break;
  case 'd':
   printf("HAVE option: -d\n");
   break;
  case 'e':
   printf("HAVE option: -e\n");
   printf("The argument of -e is %s\n", optarg);
   break;
  case '?':
   printf("Unknown option: %c\n",(char)optopt);
   break;
    }
}
```

- 编译执行命令：

```c
./main -b 123
```

- 输出结果：

```c
optind:1,opterr：1
--------------------------
optind: 3
HAVE option: -b
The argument of -b is 123
```

- 结果分析：

首先看到optind和opterr的初始值都为1，前面提到过opterr非零表示产生的错误要输出到stderr上，<font color=red>那么optind的初值为什么是1呢？</font>

我们都知道main函数两个参数argc和argv[]，argc最小值是1，代表main执行文件本身，argv[0]存放mian函数的名称及完整路径，而getopt函数前两个参数与main函数一致，optind进行检索的时候是从argv[1]开始的，所以optind初始值为1。

当执行getopt()时，optind会进行三次扫描，首先从argv[1]即-b开始检索，-b存在optstring选项字符串中，满足case ‘b’，而b后面有单冒号，表明-b后面必须要有参数，所以optind进行第二次检索扫描，argv[2]的123便是-b的参数，所以结果中输出：

```c
HAVE option: -b
The argument of -b is 123
```
为什么optind是3呢？这是因为optind是下一次选项检索的开始索引，也就是下一次getopt进行扫描从argv[3]开始，但示例中没有argv[3]，所以getopt()就会返回-1终止while循环。

- 编译执行命令：

```c
./main b 123
```

- 输出结果：

```c
optind:1,opterr：1
--------------------------
```

- 结果分析：

在字符b前去掉了‘-’，输出结果就完全不同，这是因为optind进行扫描检索的时候会以‘-’作为判断，当检索到‘-’后才会对后面的字符进行比对判断。

示例代码二:

```c
printf("--------------------------\n");
for(i=0;i<argc;i++)
{
    printf("%s\n",argv[i]);
}
printf("--------------------------\n");
printf("optind:%d,opterr：%d\n",optind,opterr);
printf("--------------------------\n");
while ((ch = getopt(argc, argv, "ab:c:de::")) != -1)
{
 printf("optind: %d\n", optind);
 switch (ch)
 {
  case 'a':
   printf("HAVE option: -a\n\n");
   break;
  case 'b':
   printf("HAVE option: -b\n");
   printf("The argument of -b is %s\n", optarg);
   break;
  case 'c':
   printf("HAVE option: -c\n");
   printf("The argument of -c is %s\n", optarg);
   break;
  case 'd':
   printf("HAVE option: -d\n");
   break;
  case 'e':
   printf("HAVE option: -e\n");
   printf("The argument of -e is %s\n", optarg);
   break;
  case '?':
   printf("Unknown option: %c\n",(char)optopt);
   break;
  }
    }
printf("----------------------------\n");
printf("optind=%d,argv[%d]=%s\n",optind,optind,argv[optind]);
printf("--------------------------\n");
for(i=0;i<argc;i++)
{
    printf("%s\n",argv[i]);
}
printf("--------------------------\n");
```

- 编译执行命令：

```c
./main zhong -b 'win' xing -e123
```

- 输出结果：

```c
--------------------------
./main
zhong
-b
win
xing
-e123
--------------------------
optind:1,opterr：1
--------------------------
optind: 4
HAVE option: -b
The argument of -b is win
optind: 6
HAVE option: -e
The argument of -e is 123
----------------------------
optind=4,argv[4]=zhong
--------------------------
./main
-b
win
-e123
zhong
xing
--------------------------
```

- 结果分析：

仔细对比下前后输出结果会发现，前后数组打印出来的顺序变化了，在getopt()执行过程中，‘win’是-b的参数，123是-e的参数，因为e后面是双冒号‘::’，所以-e和123必须是连成一个整体，zhong和xing未被检索， 所以<font color=blue>getopt函数会改变argv[]中数组顺序，将能被检索的选项字符串排列到数组前面！</font>

--------------------------------

## 2. getopt_long函数

### 2.1 介绍

getopt_long函数与getopt函数对比稍微有些出入，getopt函数只能处理短选项字符串，比如上文介绍的‘-a’或‘-b123’这类型，不能处理长选项字符串，如‘--list’或者‘--help’这种，而getopt_long函数就可以。简单的说法是getopt_long是getopt函数的增强版，它不但可以处理短选项字符串‘-’，也可以处理长选项字符串‘--’。

getopt_long函数定义如下：

```c
int getopt_long(int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);
```

对比发现，getopt_long的参数前三项和getopt一样，多了另外两个参数：

```c
const struct option *longopts;     /*指明长参数的名称和属性*/
int *longindex；                   /*如果longindex设定不为NULL，则longindex会标记optind在longopts结构体中找到的长选项字符串，也就是longopts的下标值*/
```

注意：在longopts中最后一项必须全是0，否则会报段错误。

下面会通过示例代码来详细说下这两个参数的用法，先看下option结构体：

```c
struct option
{
const char  *name;       /* 长参数名称 */
int          has_arg;    /* 指明是否带有参数 */
int          *flag;      /* flag是一个指针变量，这个指针变量为空（NULL），那么它会返回参数val的值；如果不变空，则flag指向的内存空间存储val的值，并且它返回0 */
int          val;        /* 指定的默认值，用于指定函数找到选项的返回值或flag非空时指定内存*flag的值，注意不要写-1到val，否则会导致getopt_long返回-1时程序停止解析后续的选项字符串； */
};
```

has_arg有3个值，no_argument(0)不带参数，require_argument(1)带参数值，optional_argument(2)参数可选择，可以带参数，也可以不带参数，如下：

```c
#define no_argument        0
#define required_argument  1
#define optional_argument  2
```

### 2. 示例说明

```c
int opt;
int option_index = 1;
char *optstring = "a:b:c:d";
static struct option long_options[] =
{
 {"reqarg", required_argument, NULL, 'r'},
 {"noarg",  no_argument,       NULL, 'n'},
 {"optarg", optional_argument, NULL, 'o'},
 {0, 0, 0, 0}
};
while( (opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
{
    printf("......................\n");
    printf("opt = %c\n", opt);
 printf("optarg = %s\n", optarg);
 printf("optind = %d\n", optind);
 printf("argv[optind - 1] = %s\n",  argv[optind - 1]);
 printf("option_index = %d\n", option_index);
    printf("......................\n");
}
```

- 编译执行命令：

```c
./long -a 100 --reqarg 200 --nonarg
```

- 输出结果：

```c
......................
opt = a
optarg = 100
optind = 3
argv[optind - 1] = 100
option_index = 1
......................
......................
opt = r
optarg = 200
optind = 5
argv[optind - 1] = 200
option_index = 0
......................
......................
opt = n
optarg = (null)
optind = 6
argv[optind - 1] = --noarg
option_index = 1
......................
```

- 结果分析：

在optstring中定义了短选项字符串，同时在long_options中规定了长选项字符串，首先结合上文可以分析：当optind从argv[1]处开始检索，检索到单杠号‘-’，便在optstring中找到a，并确认a后有冒号说明选项带参数，argv[2]中储存的100数值就是-a的参数；返回值就是检索到的a赋给opt；optind随之跳转到下一个位置3；因为option_index仅记录optind在long_options中找到的选项字符串，所以此次值保持不变，仍为1。

接着optind检索到双杠号‘--’，在long_options中检索到reqarg，并确认带参数，argv[4]中储存的200便是--regarg的参数；flag为NULL,返回r赋给opt；optind跳转到下一个位置为5；option_index指向optind检索在long_options中第一个字符reqarg的下标，值变为0。

optind从argv[5]处检索‘--’，在long_options中检索到nonarg，确认不带参数；flag为NULL，返回n赋给opt；optind跳转到argv[6]，但后面没有执行指令，返回-1退出循环，程序结束；而option_index指向optind检索在long_options中第二个字符nonarg的下标，值变为1。

### 2.3 对比总结

#### 2.3.1 getopt与getopt_long对比

- getopt函数的使用只需要加头文件<unistd.h>，使用getopt_long还需要额外添加头文件<getopt.h>;
- getopt_long除了会接受长选项，其他概念和getopt是一样的；
- 如果使用getopt_long想只接受短选项，设置longopts和都为NULL即可；如果只想接受长选项，相应地设置optstring为NULL即可；
- 长选项名是可以使用缩写方式，比如：选项--reqarg，那么输入--r、--re、--req等均会被正确识别为reqarg选项（可以自行用上述代码测试）；
- 对于带参数的长选项格式是：--arg=param或--arg param；
- longopts是指向struct option数组的第一个元素的指针，struct option定义在<getopt.h>中；
- longindex如果非NULL，则是返回识别到struct option数组中元素的位置指针，如上文中的reqarg和nonarg的下标。

#### 2.3.2 getopt_long与getopt_long_only对比

- getopt_long_only函数定义与getopt_long一摸一样，使用方法也一致，唯一的区别在于，使用getopt_long_only函数时，对于长选项字符串不需要使用双杠号‘--’，只用单杠号‘-’就行（可以自行用上述代码测试）。