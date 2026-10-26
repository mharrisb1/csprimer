#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main() {
  int stat;
  srand(time(NULL));

  int* np = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                MAP_ANON | MAP_SHARED, -1, 0);
  if (fork() == 0) {
    // child
    *np = rand();
    printf("Child has written %i to address %p\n", *np, np);
    exit(0);
  } else {
    // parent
    wait(&stat);
    printf("Parent has read %i from address %p\n", *np, np);

    printf("pid: %d\n", getpid());
    for (;;);
  }
}
