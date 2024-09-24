#include "kernel/types.h"
#include "user/user.h"

/*void filter(int p) {
  int n, prime;
  if (read(p, &prime, 4) != 4) {
    fprintf(2, "read error\n");
    exit(1);
  }
  printf("prime %d\n", prime);
  int fd[2];

  int flag = 0;
  while (read(p, &n, 4) == 4) {
    if (flag == 0) {
      pipe(fd);
      flag = 1;
      if (fork() == 0) {
        close(fd[1]);
        filter(fd[0]);
        return;
      } else {
        close(fd[0]);
      }
    }

    if (n % prime != 0) {
      write(fd[1], &n, 4);
    }
  }
  if (flag) {
    close(fd[1]);
    wait(0);
  }
  close(p);
  exit(0);
}

int main(int argc, char *argv[]) {
  int p[2];
  pipe(p);

  if (fork() != 0) {
    // parent -- generating numbers
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
  } else {
    close(p[1]);
    filter(p[0]);
    close(p[0]);
  }
  exit(0);
  return 0;
}
*/

// handling looping behavior of the second and subsequent subprocess
void helper(int p[2]) {
  // close the write end of the pipe which we don't need
  close(p[1]);
  int prime, n;
  if (read(p[0], &prime, 4) != 4) {
    fprintf(2, "Error in read\n");
    exit(1);
  }
  printf("prime %d\n", prime);
  if (read(p[0], &n, 4) == 4) {
    int fd[2];
    pipe(fd);
    if (fork() != 0) {
      close(fd[0]);
      if (n % prime != 0) {
        write(fd[1], &n, 4);
      }
      while (read(p[0], &n, 4) == 4) {
        if (n % prime != 0) {
          write(fd[1], &n, 4);
        }
        close(p[0]);
        close(fd[1]);
        wait(0);
      }
    } else {
      helper(fd);
    }
  }
  exit(0);
}

int main(int argc, char *argv) {
  int p[2];
  pipe(p);
  // generating numbers
  if (fork() == 0) {
    helper(p);
  } else {
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
      if (write(p[1], &i, 4) != 4) {
        fprintf(2, "failed to write %d into the pipe\n", i);
        exit(1);
      }
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
  return 0;
}
