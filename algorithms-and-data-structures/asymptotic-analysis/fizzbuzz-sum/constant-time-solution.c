#include <assert.h>

int fizzbuzz_sum(int n);

int main () {
  assert(fizzbuzz_sum(0) == 0);
  assert(fizzbuzz_sum(1) == 0);
  assert(fizzbuzz_sum(2) == 0);
  assert(fizzbuzz_sum(3) == 0);
  assert(fizzbuzz_sum(4) == 3);
  assert(fizzbuzz_sum(5) == 3);
  assert(fizzbuzz_sum(6) == 8);
  assert(fizzbuzz_sum(7) == 14);
  assert(fizzbuzz_sum(8) == 14);
  assert(fizzbuzz_sum(9) == 14);
  assert(fizzbuzz_sum(10) == 23);
  assert(fizzbuzz_sum(11) == 33);
  assert(fizzbuzz_sum(12) == 33);
  assert(fizzbuzz_sum(13) == 45);
  assert(fizzbuzz_sum(14) == 45);
  assert(fizzbuzz_sum(15) == 45);
  assert(fizzbuzz_sum(16) == 60);
   
  assert(fizzbuzz_sum(10) == 23);
  assert(fizzbuzz_sum(100) == 2318);
  assert(fizzbuzz_sum(1000) == 233168);
  assert(fizzbuzz_sum(10000) == 23331668);

  assert(fizzbuzz_sum(20) == 78);
  assert(fizzbuzz_sum(200) == 9168);
  assert(fizzbuzz_sum(2000) == 931668);
  assert(fizzbuzz_sum(20000) == 93316668);

  for (int i = 0; i < 10000; i++) {
    fizzbuzz_sum(i);
  }
}

int triangle_num(int n) {
  return (n * (n + 1)) / 2;
}

int fizzbuzz_sum(int n) {
  if (n <= 0) return 0;
  
  int stop = n - 1;
  int k = 0;
  k += (3 * triangle_num((stop - (stop % 3)) / 3));
  k += (5 * triangle_num((stop - (stop % 5)) / 5));
  k -= (15 * triangle_num((stop - (stop % 15)) / 15));
  return k;
}
