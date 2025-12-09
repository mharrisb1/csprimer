#include <stddef.h>
#include <stdio.h>

extern int f(int x);

void test_case(int n, int expected, int actual) {
  printf("[%s] f(%i) = %i (expected) = %i (actual)\n",
         expected == actual ? "✅" : "❌", n, expected, actual);
}

int main(void) {
  int tests[][2] = {
      {1, 1},         {2, 2},         {3, 4},        {4, 7},
      {5, 13},        {6, 24},        {7, 44},       {8, 81},
      {9, 149},       {10, 274},      {11, 504},     {12, 927},
      {13, 1705},     {14, 3136},     {15, 5768},    {16, 10609},
      {17, 19513},    {18, 35890},    {19, 66012},   {20, 121415},
      {21, 223317},   {22, 410744},   {23, 755476},  {24, 1389537},
      {25, 2555757},  {26, 4700770},  {27, 8646064}, {28, 15902591},
      {29, 29249425}, {30, 53798080},
  };

  size_t test_size = sizeof(tests) / sizeof(int[2]);

  for (size_t i = 0; i < test_size; i++) {
    int n        = tests[i][0];
    int expected = tests[i][1];
    int actual   = f(n);
    test_case(n, expected, actual);
  }
}
