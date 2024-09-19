#include <assert.h>
#include <stddef.h>
#include <stdio.h>

void reduce(void **arr, size_t count, size_t size,
            void *(*f)(void *acc, void *curr), void *init) {
  for (size_t i = 0; i < count; i++) {
    void *curr = (char *)arr + (i * size);
    (*f)(init, curr);
  }
}

void _i32_add(void *acc, void *curr) { *(int *)acc += *(int *)curr; }
void _i64_add(void *acc, void *curr) { *(long *)acc += *(long *)curr; }
void _f32_add(void *acc, void *curr) { *(float *)acc += *(float *)curr; }
void _f64_add(void *acc, void *curr) { *(double *)acc += *(double *)curr; }

void test_sum(const int *arr_i32, const long *arr_i64, const float *arr_f32,
              const double *arr_f64) {
  int    w = 0;
  long   x = 0;
  float  y = 0.0;
  double z = 0.0;

  reduce((void *)arr_i32, 4, sizeof(int), (void *)_i32_add, (void *)&w);
  reduce((void *)arr_i64, 4, sizeof(long), (void *)_i64_add, (void *)&x);
  reduce((void *)arr_f32, 4, sizeof(float), (void *)_f32_add, (void *)&y);
  reduce((void *)arr_f64, 4, sizeof(double), (void *)_f64_add, (void *)&z);

  assert(w == 10);
  assert(x == 10);
  assert(y < 10.01 && y > 9.99);
  assert(z < 10.01 && z > 9.99);
}

void _i32_mul(void *acc, void *curr) { *(int *)acc *= *(int *)curr; }
void _i64_mul(void *acc, void *curr) { *(long *)acc *= *(long *)curr; }
void _f32_mul(void *acc, void *curr) { *(float *)acc *= *(float *)curr; }
void _f64_mul(void *acc, void *curr) { *(double *)acc *= *(double *)curr; }

void test_product(const int *arr_i32, const long *arr_i64, const float *arr_f32,
                  const double *arr_f64) {
  int    w = 1;
  long   x = 1;
  float  y = 1.0;
  double z = 1.0;

  reduce((void *)arr_i32, 4, sizeof(int), (void *)_i32_mul, (void *)&w);
  reduce((void *)arr_i64, 4, sizeof(long), (void *)_i64_mul, (void *)&x);
  reduce((void *)arr_f32, 4, sizeof(float), (void *)_f32_mul, (void *)&y);
  reduce((void *)arr_f64, 4, sizeof(double), (void *)_f64_mul, (void *)&z);

  assert(w == 24);
  assert(x == 24);
  assert(y < 24.01 && y > 23.99);
  assert(z < 24.01 && z > 23.99);
}

int main() {
  int    arr_i32[] = {1, 2, 3, 4};
  long   arr_i64[] = {1, 2, 3, 4};
  float  arr_f32[] = {1., 2.0, 3.0, 4.0};
  double arr_f64[] = {1.0, 2.0, 3.0, 4.0};

  test_sum(arr_i32, arr_i64, arr_f32, arr_f64);
  test_product(arr_i32, arr_i64, arr_f32, arr_f64);

  printf("✅ All tests passed\n");
}
