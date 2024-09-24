/*#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char* readline() {
  char* buf = malloc(100 * sizeof(char));
  char* p = buf;

  while (read(0, p, 1) == 1) {
    if (*p == '0' || *p == '\n') {
      *p = '0'; // 统一末尾标志符
      return buf;
    }
    p++;
  }
  return 0;
}

char** split(char* obj, char mark) {
  char** buf = malloc(100 * sizeof(char*));
  char** temp = buf;
  char* p = *temp;
  while (*obj != '0') {
    if (*obj == mark) {
      temp++;
      p = *temp;
      obj++;
      continue;
    }
    *p = *obj;
    p++;
    obj++;
  }
  temp++;
  *temp = 0;
  return buf;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: xargs [command]\n");
    exit(1);
  }

  char* buf;
  char** p;
  char** arg = malloc(100 * sizeof(char*));
 
  while ((buf = readline()) != 0) {
    p = split(buf, ' ');
    printf("%s", *p);
    int pid = fork();
    if (pid == 0) {
      int i;
      argv++;
      for (i = 0; i < argc - 1; i++) {
         *(arg + i) = *argv++;
      }
      while (*p != 0) {
        *(arg + i) = *p++;
        i++;
      }
      exec(arg[0], arg);
    } else {
      wait(0);
    }
  }
  exit(0);
}*/
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
  // count -> lineSplit
  // m -> buf
  int i, count = 0, k, m = 0;
  char* lineSplit[MAXARG], *p;

  // lineSplit -> points to arguments
  // buf -> arguments
  // p -> backup buf
  char block[32], buf[32];
  p = buf;
 
  // xargs 自带参数
  for (i = 1; i < argc; i++) {
    lineSplit[count++] = argv[i];
  }

  while ((k = read(0, block, sizeof(block))) > 0) {
    for (i = 0; i < k; i++) {
      if (block[i] == '\n') {
        buf[m] = 0;
        lineSplit[count++] = p;
        lineSplit[count] = 0;
        m = 0;
        p = buf;
        count = argc - 1;
        if (fork() == 0) {
          exec(argv[1], lineSplit);
        }
        wait(0);
      } else if (block[i] == ' ') {
        buf[m++] = 0;
        lineSplit[count++] = p;
        p = &buf[m];
      } else {
        buf[m++] = block[i];
      }
    }
  }
  exit(0);
}
