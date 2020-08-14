## main函数

main函数的两种标准写法

int main(void)
int main(int argc, char *argc[]) 或者 int main(int argc, char **argv)

### main函数传参的使用

main函数传参的参数用空格来间隔的，参数之间多个空格仅会当作一个空格来处理
main函数传参时一定要检验argc，否则会造成argv字符串数组溢出，出现段错误

### main函数传参的本质

fork函数
exec函数族

### void类型介绍

编程语言分为强语言类型和弱语言类型，C语言是强语言类型

### 数据类型的本质

### void类型的本质

### void类型的使用

malloc函数举例

void类型最终归宿都是被强制类型转换成其他类型

## 扩展之NULL

NULL的宏定义

#ifdef _cplusplus
#define NULL 0
#else
#define NULL (void *)0
#endif

野指针
悬空指针

NULL使用惯例

if(NULL == P) 而不要写成 if(p == NULL)

\0 '0' 0 和NULL区别