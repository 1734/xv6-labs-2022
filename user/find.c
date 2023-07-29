// clang-format off
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
// clang-format on

void find(const char *dirPath, const char *fileName) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(dirPath, 0)) < 0) {
    fprintf(2, "ls: cannot open %s\n", dirPath);
    return;
  }
  strcpy(buf, dirPath);
  p = buf + strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    // inode number
    if (de.inum == 0) {
      continue;
    }
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0) {
      printf("ls: cannot stat %s\n", buf);
      continue;
    }
    switch (st.type) {
    case T_DEVICE:
    case T_FILE:
      if (!strcmp(de.name, fileName)) {
        printf("%s\n", buf);
      }
      break;
    case T_DIR:
      if (strcmp(de.name, ".") && strcmp(de.name, "..")) {
        find(buf, fileName);
      }
      break;
    default:
      break;
    }
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "usage: find filename dirpath\n");
    exit(1);
  }
  struct stat st;
  if (stat(argv[1], &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", argv[1]);
    exit(1);
  }
  if (st.type != T_DIR) {
    fprintf(2, "find: type of %s is not directory\n", argv[1]);
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
