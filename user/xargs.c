#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if(argc < 3) {
    fprintf(2, "usage: xagrs <command> <arguments>\n");
    exit(1);
  }
  // char *cmd = argv[1];
  char line[128];
  char ch;
  int index = 0;
  // char str[128];
  // int count = read(0, str, sizeof(str));
  // for(int i = 0 ; i < 14; i++) {
  //   printf("%c\n", str[i]);
  // }
  // printf("%s  %d\n", str, count);
  while(read(0, &ch, sizeof(ch)) > 0) {
    // printf("%c\n", ch);
    // 读取\n字符会分为两部分？如何解决
    if(ch == '\n' || ch == ' ') {
      char *args[4];
      char *cmd = argv[1];
      args[0] = argv[1];
      args[1] = argv[2];
      args[2] = line;
      args[3] = 0;
      if(fork() == 0) {
        // 子进程
        exec(cmd, args);
        exit(0);
      }else {
        wait(0);
      }
      memset(line, 0 , sizeof(line));
      index = 0;
    }else{
      line[index++] = ch;
    }
  }
  exit(0);
}