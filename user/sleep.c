#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int
main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("parameter error!\n");
    printf("Try 'sleep [time]'");
    exit(0);
  }

  sleep(atoi(argv[1]));
  exit(0);
}
