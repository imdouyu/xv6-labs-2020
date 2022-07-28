#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
create_new_process(int parent_fd[2]) {
  close(parent_fd[1]);
  int p;
  read(parent_fd[0], &p, sizeof(int));
  printf("prime %d\n", p);

  int child_fd[2];
  pipe(child_fd);
  int n;
  if(read(parent_fd[0], &n, sizeof(int)) == 0) {
    close(parent_fd[1]);
    exit(0);
  }
  if(fork() == 0) {
    create_new_process(child_fd);
    exit(0);
  }else{
    close(child_fd[0]);
    do{
      if(n % p != 0) {
        write(child_fd[1], &n, sizeof(int));
      }
    }while(read(parent_fd[0], &n, sizeof(int)) != 0);
    close(child_fd[1]);
    close(parent_fd[0]);
    wait(0);
    exit(0);
  }
}

// https://swtch.com/~rsc/thread/
int
main(int argc, char *argv[])
{
  int parent_fd[2];
  pipe(parent_fd);
  if(fork() == 0) {
    create_new_process(parent_fd);
    exit(0);
  }else {
    close(parent_fd[0]);
    for(int i = 2; i <= 35; i++) {
      write(parent_fd[1], &i, sizeof(int));
    }
    close(parent_fd[1]);
    wait(0);
    exit(0);
  }
}
