// clang-format off
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "user/user.h"
// clang-format on

#define MAXARGSIZE 512

char whitespace[] = " \t";

void parseLine(char* line, char* argv[]) {
  char* begin = line;
  char* end = 0;
  int argIndex = 0;
  while (1) {
    while (*begin != '\0' && strchr(whitespace, *begin)) {
      ++begin;
    }
    if (*begin == '\0') {
      break;
    }
    end = begin;
    while (*end != '\0' && !strchr(whitespace, *end)) {
      ++end;
    }
    argv[argIndex] = (char*)malloc(end - begin + 1);
    memset(argv[argIndex], '\0', end - begin + 1);
    memmove(argv[argIndex], begin, end - begin);
    ++argIndex;
    begin = end;
  }
  argv[argIndex] = 0;
}

int main(int argc, char* argv[]) {
  char* argvSub[MAXARG];
  for (int i = 1; i < argc; ++i) {
    argvSub[i - 1] = argv[i];
  }
  char buf[MAXARGSIZE];
  memset(buf, '\0', sizeof(buf));
  char* p = buf;
  int subProcessNumber = 0;
  int readByteNumber = -1;
  while (1) {
    readByteNumber = read(0, p, 1);
    if (readByteNumber == 0 || *p == '\n') {
      *p = '\0';
      parseLine(buf, argvSub + argc - 1);
      if (fork() == 0) {
        printf("%s\n", argvSub[0]);
        exec(argvSub[0], argvSub);
      }
      ++subProcessNumber;
      if (readByteNumber == 0) {
        close(0);
        break;
      }
      p = buf;
      memset(buf, '\0', sizeof(buf));
    } else {
      ++p;
    }
  }
  for (int i = 0; i < subProcessNumber; ++i) {
    wait(0);
  }
  exit(0);
}
