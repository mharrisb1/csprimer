#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MAXBUF 256

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef struct {
  int   decimal;
  char *roman;
} roman_tuple_t;

static const roman_tuple_t mapping[] = {
    {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
    {90, "XC"},  {50, "L"},   {40, "XL"}, {10, "X"},   {9, "IX"},
    {5, "V"},    {4, "IV"},   {1, "I"},
};

static const int mapping_size = ARRAY_LENGTH(mapping);

static const roman_tuple_t tests[] = {
    {1, "I"},
    {2, "II"},
    {3, "III"},
    {4, "IV"},
    {5, "V"},
    {6, "VI"},
    {7, "VII"},
    {8, "VIII"},
    {9, "IX"},
    {10, "X"},
    {11, "XI"},
    {12, "XII"},
    {13, "XIII"},
    {14, "XIV"},
    {15, "XV"},
    {16, "XVI"},
    {17, "XVII"},
    {18, "XVIII"},
    {19, "XIX"},
    {24, "XXIV"},
    {29, "XXIX"},
    {34, "XXXIV"},
    {39, "XXXIX"},
    {41, "XLI"},
    {44, "XLIV"},
    {49, "XLIX"},
    {58, "LVIII"},
    {68, "LXVIII"},
    {73, "LXXIII"},
    {83, "LXXXIII"},
    {88, "LXXXVIII"},
    {94, "XCIV"},
    {99, "XCIX"},
    {101, "CI"},
    {111, "CXI"},
    {149, "CXLIX"},
    {198, "CXCVIII"},
    {242, "CCXLII"},
    {399, "CCCXCIX"},
    {400, "CD"},
    {444, "CDXLIV"},
    {499, "CDXCIX"},
    {500, "D"},
    {944, "CMXLIV"},
    {999, "CMXCIX"},
    {1000, "M"},
    {1453, "MCDLIII"},
    {1666, "MDCLXVI"},
    {1776, "MDCCLXXVI"},
    {1888, "MDCCCLXXXVIII"},
    {1918, "MCMXVIII"},
    {1944, "MCMXLIV"},
    {1954, "MCMLIV"},
    {1984, "MCMLXXXIV"},
    {1999, "MCMXCIX"},
    {2008, "MMVIII"},
    {2421, "MMCDXXI"},
    {2759, "MMDCCLIX"},
    {2999, "MMCMXCIX"},
    {3888, "MMMDCCCLXXXVIII"},
    {3999, "MMMCMXCIX"},
};

static const int test_size = ARRAY_LENGTH(tests);

int roman(char *buf, size_t buf_size, int decimal) {
  char *start = buf;
  char *end   = buf + buf_size;

  for (int i = 0; i < mapping_size && buf < end; i++) {
    while (decimal >= mapping[i].decimal && buf < end - 1) {
      size_t len = strlen(mapping[i].roman);
      if (buf + len >= end) {
        *buf = '\0';
        return -1;
      }
      strcpy(buf, mapping[i].roman);
      buf += len;
      decimal -= mapping[i].decimal;
    }
  }
  return buf - start;
}

int main() {
  char          buf[MAXBUF];
  int           failures = 0;
  roman_tuple_t expected;

  for (int i = 0; i < test_size; i++) {
    expected = tests[i];

    int len = roman(buf, MAXBUF, expected.decimal);

    if (len < 0) {
      printf("[\e[1;31mERROR\e[0m] Buffer overflow for %d\n", expected.decimal);
      failures++;
    } else if (strcmp(buf, expected.roman) != 0) {
      printf("[\e[1;31mERROR\e[0m] %s != %s\n", buf, expected.roman);
      failures++;
    }
  }

  if (failures > 0) {
    printf("\n%i tests failed!\n\n", failures);
  } else {
    printf("\n[\e[1;32mPASSED\e[0m] All tests passed!\n\n");
  }

  return failures > 0 ? 1 : 0;
}
