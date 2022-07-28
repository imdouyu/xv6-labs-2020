#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc > 1) {
    fprintf(2, "usage: pingpong \n");
    exit(1);
  }
  // fd[0]: read end of pipe
  // fd[1]: write end of pipe
  int parent_fd[2];
  int child_fd[2];
  // unidirectional is a unidirectional data channel
  pipe(parent_fd);
  pipe(child_fd);
  if(fork() == 0) {
    // child
    char buf[10];
    read(parent_fd[0], buf, sizeof(buf));
    
    int pid = getpid();
    if(buf[0] == 'K')
        fprintf(1, "%d: received ping\n", pid);
    
    write(child_fd[1], "O", sizeof(buf));
  }else{
    // parent
    char buf[10];
    write(parent_fd[1], "K", sizeof(buf));
    
    read(child_fd[0], buf, sizeof(buf));
    int pid = getpid();
    if(buf[0] == 'O')
        fprintf(1, "%d: received pong\n", pid);
  }
  exit(0);
}
// fd:
// 0 stdin
// 1 stdout
// 2 stderr
