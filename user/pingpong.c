#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int
main(int argc, char *argv[]) {
  int p[2];
  pipe(p);
  if(fork() != 0) {
    char buf[1];
    buf[0] = 'a';
    write(p[1], buf, 1);
    close(p[1]);
    if(read(p[0], buf , 1) == 1) {
      fprintf(1, "%d: received pong\n", getpid());
    }
  } else {
    char buf[1];
    if(read(p[0], buf, 1)  == 1) {
      close(p[0]);
      fprintf(1, "%d: received ping\n", getpid());
      write(p[1], buf, 1);
    }
  }
  exit(0);
}
