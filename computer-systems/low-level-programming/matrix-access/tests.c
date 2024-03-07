#include <assert.h>
#include <stdio.h>

extern int index(int *matrix, int rows, int cols, int rindex, int cindex);

int main(void) {

  int matrix1[1][4] = {{1, 2, 3, 4}};
  assert(index((int *)matrix1, 1, 4, 0, 2) == 3);

  int matrix2[4][1] = {{1}, {2}, {3}, {4}};
  assert(index((int *)matrix2, 4, 1, 1, 0) == 2);

  int matrix3[2][3] = {{1, 2, 3}, {4, 5, 6}};
  assert(index((int *)matrix3, 2, 3, 1, 2) == 6);

  printf("OK\n");
}
