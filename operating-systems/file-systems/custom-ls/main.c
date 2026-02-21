#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <PATH>", argv[0]);
    exit(EXIT_FAILURE);
  }

  DIR *dirp = opendir(argv[1]);
  if (dirp == NULL) {
    exit(EXIT_FAILURE);
  }

  struct dirent *dp;
  while ((dp = readdir(dirp)) != NULL) {
    printf("%lu %s\n", dp->d_ino, dp->d_name);
  }

  closedir(dirp);
  
  exit(EXIT_SUCCESS);
}
