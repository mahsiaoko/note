#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
int main(int argc, char *argv[])
{
  int opt; /*接收选项*/
  // extern char *optarg; /*指向当前getopt()返回选项的参数*/
  // extern int optopt;   /*当选项没有出现在optstring中，或者选项缺少必要的参数时，该选项存储在optopt中，getopt返回'？’*/
  // extern int opterr;   /*用于控制getopt()是否打印出错信息*/
  // extern int optind;   /*当前getopt()返回选项的下一个选项的索引(argv数组)*/

  // opterr = 0; /*不要打印出错信息*/
  opterr = 0;

  while ((opt = getopt(argc, argv, "a1b:c::")) != -1)
  {
    /* a和1为不带参数选项,b为必须带一个参数选项,c为可选参数选项(注意参数与-c直接不能分开) */
    /* 示例： getopt -a -b 100 -c12 */
    switch (opt)
    {
    case 'a':
    case '1':
      printf("选项: %c\n", opt);
      break;
    case 'b':
      printf("选项: b，带的参数是 %s\n", optarg);
      break;
    case 'c':
      printf("选项: c，带的参数是 %s\n", optarg);
      break;
    default: /* '?' */
      if (optopt == 'c')
      {
        printf("选项: c，没有带参数\n");
        break;
      }
      fprintf(stderr, "用法: %s [-1a] [-c [argument]] [-b argument]\n",
              argv[0]);
      exit(EXIT_FAILURE); //无效的参数，退出程序
    }
  }
  printf("optind=%d\n", optind);

  //在命令行选项参数再也检查不到optstring中包含的选项时，
  //返回－1，同时optind储存第一个不包含选项的命令行参数。
  //getopt 中指的 选项是指以 `-`开头的
  if (optind >= argc)
  {
    fprintf(stderr, "选项索引超过了argv数组的长度\n");
    exit(EXIT_FAILURE);
  }
  //输出第一个不包含选项的参数
  printf("非选项参数 = %s\n", argv[optind]);

  //输出一下命令行参数，看看是否被改变
  for (opt = 0; opt < argc; ++opt)
  {
    printf("索引:%d\t\t命令行参数:%s\n", opt, argv[opt]);
  }

  exit(EXIT_SUCCESS); //成功退出
}