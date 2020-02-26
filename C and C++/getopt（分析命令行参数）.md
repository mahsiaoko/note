# getopt（分析命令行参数）

## 相关函数表头文件

`#include <unistd.h>`

## 函数声明

`int getopt(int argc,char*const argv[],const char * optstring);`

## 全局变量

```c
  extern char *optarg;

　extern int optind, opterr, optopt;  //索引/错误输出标志/最后一个未知选项
```

## 函数说明

getopt()用来分析命令行参数。参数 argc 和 argv 是由 main()传递的参数个数和内容。参数 optstring 则代表欲处理的选项字符串。此函数会返回在 argv 中下一个的选项字母，此字母会对应参数 optstring 中的字母。如果选项字符串里的字母后接着冒号“:”，则表示还有相关的参数，全域变量 optarg 即会指向此额外参数。如果 getopt()找不到符合的参数，则会打印出错信息，并将全域变量 optarg 设为“?”字符，如果不希望 getopt()打印出错信息，则只要将全域变量 opterr 设为 0 即可。

getopt() 所设置的全局变量包括：

char \*optarg——当前选项参数字串（如果有）。

int optind——argv 的当前索引值。当 getopt()在 while 循环中使用时，循环结束后，剩下的字串视为操作数，在 argv[optind]至 argv[argc-1]中可以找到。

int opterr——这个变量非零时，getopt()函数为“无效选项”和“缺少参数选项，并输出其错误信息。

int optopt——当发现无效选项字符之时，getopt()函数或返回'?'字符，或返回':'字符，并且 optopt 包含了所发现的无效选项字符。

## 短参数的定义

getopt()使用 optstring 所指的字串作为短参数列表，象"1ac:d::"就是一个短参数列表。短参数的定义是一个-后面跟一个字母或数字，象-a,-b 就是一个短参数。每个数字或字母定义一个参数。

其中短参数在 getopt 定义里分为三种：

1.不带值的参数，它的定义即是参数本身。

2.必须带值的参数，它的定义是在参数本身后面再加一个冒号。

3.可选值的参数，它的定义是在参数本身后面加两个冒号 。

在这里拿上面的"1ac:d::"作为样例进行说明，其中的 1,a 就是不带值的参数，c 是必须带值的参数，d 是可选值的参数。
　　在实际调用中，`-1 -a -c cvalue -d, -1 -a -c cvalue -ddvalue,-1a -ddvalue -c cvalue` 都是合法的。这里需要注意三点：

1.不带值的参数可以连写，象 1 和 a 是不带值的参数，它们可以-1 -a 分开写，也可以-1a 或-a1 连写。

2.参数不分先后顺序，-1a -c cvalue -ddvalue 和-d -c cvalue -a1 的解析结果是一样的。

3.要注意可选值的参数的值与参数之间不能有空格，必须写成-ddvalue 这样的格式，如果写成-d dvalue 这样的格式就会解析错误。

默认情况下 getopt **会重新排列命令行参数的顺序**，所以到最后所有不包含选项的命令行参数都排到最后。

## 返回值

getopt()每次调用会逐次返回命令行传入的参数。
当没有参数的最后的一次调用时，getopt()将返回-1。
当解析到一个不在 optstring 里面的参数，或者一个必选值参数不带值时，返回?。
当 optstring 是以:开头时，缺值参数的情况下会返回:，而不是? 。

## 范例

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int opt;    /*接收选项*/
    extern char* optarg;/*指向当前getopt()返回选项的参数*/
    extern int optopt;  /*当选项没有出现在optstring中，或者选项缺少必要的参数时，该选项存储在optopt中，getopt返回'？’*/
    extern int opterr;  /*用于控制getopt()是否打印出错信息*/
    extern int optind;  /*当前getopt()返回选项的下一个选项的索引(argv数组)*/

    opterr = 0; /*不要打印出错信息*/

    while ((opt = getopt(argc, argv, "a1b:c::")) != -1) {
        /* a和1为不带参数选项,b为必须带一个参数选项,c为可选参数选项(注意参数与-c直接不能分开) */
        /* 示例： getopt -a -b 100 -c12 */
        switch (opt) {
            case 'a':
            case '1':
                printf("选项: %c\n",opt);
                break;
            case 'b':
                printf("选项: b，带的参数是 %s\n",optarg);
                break;
            case 'c':
                printf("选项: c，带的参数是 %s\n",optarg);
                break;
            default: /* '?' */
                if(optopt == 'c'){
                    printf("选项: c，没有带参数\n");
                    break;
                }
                fprintf(stderr, "用法: %s [-1a] [-c [argument]] [-b argument]\n",
                            argv[0]);
                exit(EXIT_FAILURE); //无效的参数，退出程序
        }
    }
    printf("optind=%d\n",optind);

    //在命令行选项参数再也检查不到optstring中包含的选项时，
    //返回－1，同时optind储存第一个不包含选项的命令行参数。
    //getopt 中指的 选项是指以 `-`开头的
    if (optind >= argc) {
        fprintf(stderr, "选项索引超过了argv数组的长度\n");
        exit(EXIT_FAILURE);
    }
    //输出第一个不包含选项的参数
    printf("非选项参数 = %s\n", argv[optind]);

    //输出一下命令行参数，看看是否被改变
    for(opt = 0; opt < argc ; ++opt){
        printf("索引:%d\t\t命令行参数:%s\n",opt,argv[opt]);
    }

    exit(EXIT_SUCCESS); //成功退出
}
```

## getopt_long

getopt_long 的原型是:

```c
int getopt_long(int argc, char * const argv[], 
        const char *optstring, 
        const struct option *longopts, 
        int *longindex);
```

这里参数 longopts，其实是一个结构的实例：

````C
struct option {
  const char *name;
  int has_arg;
  int *flag;
  int val;
}
```

1.name表示的是长参数名； 
2.has_arg有三个值供选择：
- no_argument(或者是0)，表示该参数后面不跟参数值
- required_argument(或者是1),表示该参数后面一定要跟个参数值
- optional_argument(或者是2),表示该参数后面可以跟，也可以不跟参数值
3.flag
用来决定，getopt_long()的返回值到底是什么。如果 flag 成员未设置为 NULL，在处理期间遇到此选项时，会使用val 成员的值填充它所指向的 int 值。如果 flag 成员为 NULL，在 getopt_long() 遇到此选项时，将返回 val 中的值；
4.val和flag联合决定返回值

getopt_long 示例代码

```C
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern int optind, opterr, optopt;

int main(int argc, char **argv)
{
    int c;  /* 用于接收字符选项 */
    int digit_optind = 0;   /* 用于接收数字选项 */

    while (1) {
        /* */
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        /* 长选项结构体数组 */
        static struct option long_options[] = {
            {"add",     required_argument, 0,  0 }, //需要一个参数
            {"append",  no_argument,       0,  0 }, //没有参数
            {"delete",  required_argument, 0,  0 },
            {"verbose", no_argument,       0,  0 },
            {"create",  required_argument, 0, 'c'}, //返回字符'c'
            {"file",    required_argument, 0,  0 },
            {0,         0,                 0,  0 }
        };
/*
        struct option {
            const char *name; //选项名称
            int         has_arg;    //参数标志(no_argument/0没有参数;required_argument/1需要一个参数;optional_argument/2一个可选参数)
            int        *flag;   //指定如何返回一个较长的选项。当这个指针为空的时候，函数直接将val的数值从getopt_long的返回值返回出去，当它非空时，val的值会被赋到flag指向的整型数中，而函数返回值为0
            int         val;    //用于指定函数找到该选项时的返回值，或者当flag非空时指定flag指向的数据的值。
        };
*/
        /* 获取一个选项 */
        c = getopt_long(argc, argv, "abc:d:012",
                    long_options, &option_index);

        if (c == -1){   /* 无参数可获取了 */
          break;
        }

        switch (c) {    /* 获取参数解析 */
            case 0:
                printf("选项是:%s", long_options[option_index].name);
                if (optarg){    /*如果是带参数的选项 */
                  printf(" 参数是: %s", optarg);
                }
                printf("\n");
                break;

            case '0':
            case '1':
            case '2':
                if (digit_optind != 0 && digit_optind != this_option_optind)
                  printf("digits occur in two different argv-elements.\n");
                digit_optind = this_option_optind;
                printf("选项: %c\n", c);
                break;

            case 'a':
                printf("选项: a\n");
                break;

            case 'b':
                printf("选项: b\n");
                break;
            case 'c':
                printf("选项: c 带的值: '%s'\n", optarg);
                break;

            case 'd':
                printf("选项: d 带的值: '%s'\n", optarg);
                break;

            case '?':
                break;

            default:
                printf("?? getopt 返回字符代码 0%o ??\n", c);
        }
    }

    if (optind < argc) {
        printf("非选项的命令行参数项: ");
        while (optind < argc)
          printf("%s ", argv[optind++]);
        printf("\n");
    }
    exit(EXIT_SUCCESS);
}
````
