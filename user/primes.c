#include "kernel/types.h"
#include "user/user.h"

void func(int p, int fdRead) {
  printf("prime %d\n", p);
  int n;
  int pid = -1;
  int fd[2];
  while (read(fdRead, &n, 4)) {
    if (n % p == 0) {
      continue;
    }
    if (pid > 0) {
      write(fd[1], &n, 4);
      continue;
    }
    pipe(fd);
    pid = fork();
    if (0 == pid) {
      close(fdRead);
      close(fd[1]);
      func(n, fd[0]);
    }
    close(fd[0]);
  }
  close(fdRead);
  if (pid > 0) {
    close(fd[1]);
  }
  wait(0);
  exit(0);
}

int main() {
  int fd[2];
  pipe(fd);
  int pid = fork();
  if (pid == 0) {
    close(fd[1]);
    func(2, fd[0]);
  }
  close(fd[0]);
  for (int i = 3; i <= 35; ++i) {
    write(fd[1], &i, 4);
  }
  close(fd[1]);
  wait(0);
  return 0;
}
