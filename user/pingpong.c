#include "kernel/types.h"
#include "user/user.h"

int main() {
  int fd1[2];
  pipe(fd1);
  int fd2[2];
  pipe(fd2);
  int pid = fork();
  if(pid == 0) {
    close(fd1[1]);
    close(fd2[0]);
    char* buf = (char*) malloc(1);
    read(fd1[0], buf, 1);
    printf("%d: received ping\n", getpid());
    write(fd2[1], buf, 1);
    free(buf);
    return 0;
  }
  close(fd1[0]);
  close(fd2[1]);
  char* buf = (char*) malloc(1);
  buf[0] = 'a';
  write(fd1[1], buf, 1);
  read(fd2[0], buf, 1);
  printf("%d: received pong\n", getpid());
  free(buf);
  return 0;
}