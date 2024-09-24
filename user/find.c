#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void deep_down(char *path, char *obj) {
  int fd;
  char buf[512], *p;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
    case T_FILE:
      char *pa;
      for (pa = path + strlen(path); pa >= path && *pa != '/'; pa--);
      pa++;
      if (strcmp(pa, obj) == 0) {
        printf("%s\n", path);
      }
      break;

    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        return;
      }

      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;
        else if (de.name[0] == '.' && de.name[1] == 0) continue;
        else if (de.name[0] == '.' && de.name[1] == '.' && de.name[2] == 0) continue;

        memmove(p, de.name, DIRSIZ);
        //  NOTE:
        // p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
          printf("find: cannot stat %s\n", buf);
          continue;
        }
        deep_down(buf, obj);
      }
      break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(2, "augument error\n");
    exit(1);
  }

  deep_down(argv[1], argv[2]);
  return 0;
}
