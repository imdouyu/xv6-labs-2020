#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
find(char *path, char *file) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type) {
    case T_FILE:
    // find ./a.txt a.txt
      for(p = path + strlen(path); p >= path && *p != '/'; p--) {
        ;
      }
      p++;
      if(!strcmp(p, file)) {
        fprintf(1, "%s\n", path);
      }
      break;
    case T_DIR:
    // find ./b/ a.txt
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        // 这里为什么要这么比较呢?
        printf("ls: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
    // On files that support seeking, the read operation commences at
    // the file offset, and the file offset is incremented by the number
    // of bytes read.
      while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        // ls
        // . .. otherfiles
        if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, "..")) {
          // https://stackoverflow.com/questions/2099121/why-do-inode-numbers-start-from-1-and-not-0
          // inode 0 is used as a sentinel value to indicate null or no inode 
          continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
          printf("ls: cannot stat %s\n", buf);
          continue;
        }
        find(buf, file);
      }
      break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "usage: find <path> <file>\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}