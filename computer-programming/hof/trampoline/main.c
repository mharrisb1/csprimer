// int fac(int n) { return n == 1 ? n : n * fac(n - 1); }

int fac(int n, int acc) {
  if (n == 1) {
    return acc;
  }
  __attribute__((musttail)) return fac(n - 1, n * acc);
}

int main() { int n = fac(4, 1); }
