#include <assert.h>
#include <stdio.h>

extern int mat_index(int *matrix, int rows, int cols, int rindex, int cindex);

int main(void) {

  int matrix1[1][4] = {{1, 2, 3, 4}};
  assert(mat_index((int *)matrix1, 1, 4, 0, 2) == 3);

  int matrix2[4][1] = {{1}, {2}, {3}, {4}};
  assert(mat_index((int *)matrix2, 4, 1, 1, 0) == 2);

  int matrix3[2][3] = {{1, 2, 3}, {4, 5, 6}};
  assert(mat_index((int *)matrix3, 2, 3, 1, 2) == 6);

  printf("OK\n");
}
