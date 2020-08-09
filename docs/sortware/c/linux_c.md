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

## 3. setsockopt函数

### 3.1 介绍

在socket网络通信中，通常需要设置或者获取与某个套接字关联的选项，而套接字处于多层协议中，可用setsockopt()来设置套接字选项状态，在man手册中查到函数定义及头文件如下：

```c
#include <sys/types.h>          /* 为代码更好的移植性一般会加上 */
#include <sys/socket.h>
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```

- 传参说明：

```c
sockfd: 套接字描述符
level：level可以理解为解析什么选项，包括SOL_SOCKET(套接字选项)、IPPROTO_IP(IP协议选项)、IPPROTO_TCP(TCP协议选项)，操作不同选项就要选择对应的选项协议
optname：需要访问的选项名，这部分较多，下面会用表格列出来
optval：这是一个指针变量，指向存放选项值的缓冲区，简单理解为一个缓冲区的头指针
optlen：表示optval指向的缓冲区的大小
```

optname分类如下表所示：

|level(级别)|optname(选项名)|getsockopt|setsockopt|说明|数据类型|
|:--------:|:-----------:|:----:|:----:|:----:|:----:|
|SOL_SOCKET|SO_BROADCAST    |★       |★        |允许广播方式传输数据|BOOL|
|SOL_SOCKET|SO_DEBUG        |★       |★        |允许调试|int|
|SOL_SOCKET|SO_DONTROUTE    |★       |★        |送出的数据包不要利用路由设备来传输|int|
|SOL_SOCKET|SO_ERROR        |★       |          |返回socket已发生的错误原因|int|
|SOL_SOCKET|SO_KEEPALIVE    |★       |★        |保持连接|int|
|SOL_SOCKET|SO_LINGER       |★       |★        |延迟关闭连接|struct linger|
|SOL_SOCKET|SO_OOBINLINE    |★       |★        |让接收到的带外数据继续在线留存|int|
|SOL_SOCKET|SO_RCVBUF       |★       |★        |接收缓冲区大小|int|
|SOL_SOCKET|SO_SNDBUF       |★       |★        |发送缓冲区大小|int|
|SOL_SOCKET|SO_RCVLOWAT     |★       |★        |接收缓冲区下限|int|
|SOL_SOCKET|SO_SNDLOWAT     |★       |★        |发送缓冲区下限|int|
|SOL_SOCKET|SO_RCVTIMEO     |★       |★        |接收超时|struct timeval|
|SOL_SOCKET|SO_SNDTIMEO     |★       |★        |发送超时|struct timeval|
|SOL_SOCKET|SO_REUSEADDR    |★       |★        |允许重用本地地址|BOOL|
|SOL_SOCKET|SO_TYPE         |★       |          |获得套接字类型|int|

- 返回值：

setsockopt()是int型函数，执行成功后返回0，失败返回-1，同时当函数执行失败时，根据errno可以识别失败原因：

```c
EBADF：sockfd不是有效的文件描述符
EFAULT：optval指向的内存并非有效的进程空间
EINVAL：在调用setsockopt函数时，optlen无效
ENOPROTOOPT：指定的协议层不能识别选项
ENOTSOCK：sockfd描述的不是套接字
```

### 3.2 使用

- 优雅断开和强制断开

选项名SO_LINGER的数据类型是struct linger结构：

```c
#include <arpa/inet.h>
struct linger
{
　　int l_onoff;
　　int l_linger;
};
```

struct liner结构体中两个变量l_onoff和l_linger组合使用，共可以组合成三种断开连接的方式：

1. l_onoff==0 && l=linger忽略：close()执行立刻返回，底层会将未发送完的数据继续发送后再释放资源，即优雅断开；
2. l_onoff!=0 && l_linger==0：close()执行立刻返回，底层不会发送未发送的数据，而是通过一个REST包强制关闭socket描述符，即强制断开；
3. l_onoff!=0 && l_linger>0：close()执行不会立刻返回，内核会延迟一段时间，这个时间就由l_linger的值来决定。如果timeout时间到达之前将所有的数据发送出去并接收到另一端的确认，close()会返回正确，socket描述符优雅的断开；否则close()会直接返回错误值，还未发送的数据会丢失，socket描述符被强制断开。

```c
struct linger ling = {0, 0};
setsockopt(socketfd, SOL_SOCKET, SO_LINGER, (void*)&ling, sizeof(ling));  //设置socket通信强制断开，未发送的数据直接丢弃
```

- 重用本地地址和端口

服务器的IP端口在进行socket通信时是进行bind()绑定的，所以当服务器使用close()关闭后不会立即关闭，会经历一段等待时间(TIME_WAIT,2MSL)，如果这时候还想要继续使用此socket，可以使用SO_REUSEADDR进行配置，****这个配置要放在bind函数之前**，那么在socket进入TIME_WAIT状态时可以重用此IP端口；而客户端的端口是随机分配的，使用close()关闭后不会影响下次连接：

```c
BOOL bReuseaddr=TRUE;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));
```

*注意：对于BOOL类型的选项，0表示disable，1表示enable。*

- 设置收发数据的时限

有时因为网络状况等原因，执行send()\recv()或write()\read()收发数据不能预期进行，通过SO_SNDTIMEO和SO_RCVTIMEO设置收发数据的时限：

```c
int nNetTimeout=1000;                                                              //设置1秒
setsockopt(socket, SOL_S0CKET,SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int)); 　　//发送时限
setsockopt(socket, SOL_S0CKET,SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));     //接收时限
```

- 设置收发数据的缓冲区大小

每个socket都有一个发送缓冲区和接收缓冲区，TCP和UDP通信都将接收到的数据保存在缓冲区等待进程来读取，TCP socket接收缓冲区不可能溢出，因为TCP协议有流量控制，不允许发送端发超过缓冲区大小的数据；UPD协议没有流量控制，发送端只管发送数据，接收端超出缓冲区就会被丢弃，而且发送端数据发送频繁时，接收端缓冲区内的数据会被覆盖，造成数据丢失。

可以通过SO_SNDBUF和SO_RCVBUF来设置socket通信收发数据缓冲区的大小：

```c
int nRecvBuf=32*1024;                                                             //设置为32K
setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));   //接收缓冲区
　　
int nSendBuf=32*1024;                                                             //设置为32K
setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));   //发送缓冲区
```

*注意：SO_RCVBUF缓冲区的设置，在客户端程序中要在connect()之前，在服务器程序中要在listen()之前。缓冲区的大小设置不是无限的，必须小于内核设置的上限值，可以通过`sysctl -a`来查看内核上限。*

- 设置UDP数据广播特性

```c
BOOL bBroadcast=TRUE;
setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (const char*)&bBroadcast, sizeof(BOOL));
```

## 4. getsockopt函数

getsockopt是用来获取套接字选项状态函数，其函数使用和参数介绍与setsockopt一致，就不重复介绍了。

- 函数定义：

```c
int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
```

## 5. printf、sprintf和snprintf函数

### 5.1 printf函数

printf函数是标准输出函数，功能是将参数format中的字符串进行格式化，然后将结果写出到标准输出设备(控制台、显示屏等)，直到出现字符串结束`\0`为止，通常我们在代码中用printf函数来打印log信息进行调试，根据调试信息来排查代码错误

```c
#include<stdio.h>
int printf(const char * format, ...);
```

在参数format字符串中可以包含三种字符类型：

- 一般文本，写入到双引号中，直接输出
- ASCII控制字符，如`\t`(空格符)、`\n`(换行符)等，其中`\`是一个转义字符，如果`\\`会直接输出`\`
- 格式转换字符，格式转换需要添加`%`(百分比符号)，且格式转换需要跟类型匹配起来，否则会报类型不匹配的warning，只有当使用两个`%%`时会输出`%`，如下列举一些需要注意的格式转换字符

```c
%d    参数类型为int或者*p(指针的解引用，相当于int)
%ld   参数类型为long [int]
%lld  参数类型为long long [int]
%f    参数类型为float
%lf   参数类型为double
```

### 5.2 sprintf函数

sprintf函数功能将参数format字符串进行格式化后复制到str指向的数组中，直到出现字符串结束`\0`为止，其参数format字符串的格式与printf一致，但需要注意的是：**sprintf函数格式化的数据并不会输出到输出设备，而是保存在str指向的数组中，如果需要把数据打印出来，还需要额外使用printf函数打印**

```c
#include<stdio.h>
int sprintf( char *str, const char *format, ...);
```

### 5.3 snprintf函数

snprintf函数功能上与sprintf函数一致，也是将参数format字符串进行格式化后复制到str指向的数组中，唯一不同的是：**snprintf函数最多只复制n-1个有效字符(因为`\0`会占用一个字节)，如果数据超出n-1个字节，数据就会被截断，str数组中保留前n-1个字节的数据，后面的数据会被丢弃，这一点尤为重要，snprintf函数会主动对数据长度进行截断，实际编译代码时不会因为数组溢出而报错，但是sprintf函数并不会主动截断数据长度，当str所指向的数组size小于等于实际的数据长度时，则编译代码会报数组溢出错误**

```c
#include<stdio.h>
int snprintf(char *str, size_t size, const char *format, ...);
```

## 6. putc、puts、fputc、fputs和putchar函数

### 6.1 putc函数和fputc函数

putc和fputc函数的函数定义相同，将字符c写入指定的数据流(即文件或标准输出)中，如果写入成功则返回字符c所对应的ASCII码十进制数字，写入失败则返回EOF(-1)，对于`\0`表示的空字符也一样会写入并返回ASCII对应数字0。

```c
#include <stdio.h>
int putc(int c, FILE *stream);
int fputc(int c, FILE *stream);
```

参数`int c`可以是单个char型字符，也可以是char型字符串，但putc和fputc函数每次只写入1个字符，如果需要输出字符串，则需要重复使用此函数进行写入，通常对于字符串，会使用puts或fputs函数进行替代。

参数`FILE *stream`可以写入到指定文件中，也可以写入到标准输出设备，通常为显示屏：

```c
fputc('c', fp);     //写入到fp指向的文件中
fputc('c',stdout);  //写入到屏幕上显示
```

### 6.2 puts函数和fputs函数

puts和fputs函数在函数定义稍微有些区别，puts函数功能是将字符串写入到标准输出设备中显示并换行，也就是会自动在字符串结尾添加`\n`进行换行，打印效果类似printf函数，但puts函数不可以设置输出格式化，以`\0`空字符为结束标识：

```c
#include <stdio.h>
int puts(const char *s);
```

而fputs函数则在puts函数上做了一层封装，将字符串写入到指定文件中或者标准输出设备，以`\0`空字符为结束标识，区别是fputs函数并不会在字符串结尾添加`\n`换行符，使用fputs函数需要换行则要手动添加：

```c
#include <stdio.h>
int fputs(const char *s, FILE *stream);
```

### 6.2 putchar函数

putchar函数功能类同于putc函数，将字符写入到标准输出设备：

- 定义：

```c
#include <stdio.h>
int putchar(int c);    
```

- 使用：

```c
putchar('c');      //等同于putc('c',stdout)；
```

## 7. getc、gets、fgetc、fgets、getchar和ungetc函数

### 7.1 getc函数和fgetc函数

getc函数和fgetc函数的函数定义和功能是一样的，都是从数据流(即文件或标准输入设备)中读取字符，区别在于putc函数可以作为宏来使用，而fputc函数只能作为函数来调用，实际使用中可以忽略区别，读取成功便返回所读取到的字符，读取失败则返回EOF(-1):

- 定义：

```c
#include <stdio.h>
int getc(FILE *stream);
int fgetc(FILE *stream);
```

- 使用：

```c
char str;
FILE *fp;
fp = fopen("D:\\Linux_Ubuntu\\windows_share\\test.txt","r");
str = putc(stdin);
str = fputc(fp);
```

### 7.2 gets函数和fgets函数

gets函数的功能是从标准输入设备中读取一行字符串到参数s所指向的缓冲区，以`\0`结尾，gets函数不会进行缓冲区溢出的检查，也就是说很有可能会从stdin读取的字符串长度超出缓冲区的长度。

```c
#include <stdio.h>
char *gets(char *s);
```

fgetc函数的功能是从数据流中读取size-1大小的字符串存储在参数s所指向的缓冲区中，缓冲区最后一位填充`\0`，也就是说相比于gets函数，fgets函数会进行缓冲区溢出检查，通常size的大小就是缓冲区的长度；另外gets只能从stdin中读取，而fgets函数可以从文件流中读取字符串。

```c
#include <stdio.h>
char *fgets(char *s, int size, FILE *stream);
```

### 7.3 getchar函数

getchar函数功能类似getc(stdin)，都是从stdin中读取单个字符。

```c
#include <stdio.h>
int getchar(void);
```

### 7.4 ungetc函数

ungetc函数的功能是在getc\getchar等函数从stdin中读取字符时，当这个字符不是我们想要的时候，就将这个字符退回到缓冲区，退回的字符由下一个字符读取函数来读取(这里的意思是当前的getc或getchar函数执行完成，由后续的函数来获取退回到缓冲区的字符)

- 定义：

```c
#include <stdio.h>
int ungetc(int c, FILE *stream);
```

- 使用

```c
int i = 0;
char ch;
while((ch = getchar()) != EOF && isdigit(ch))
{
    i = 10 * i + ch - 48;                         //getchar函数读取英文字母或数字并将其转为整数
}                                                
if (ch != EOF)
{
    ungetc(ch,stdin);                             //把一个非英文或数字字符退回输入流
}
```
