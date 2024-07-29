# Luhn algorithm

Implementations of the Luhn algorithm in 4 different languages:

1. Scheme ([Steel](https://github.com/mattwparas/steel) dialect)
2. C
3. A64 Assembly
4. Python (used as reference below)

## Approach

Not accounting for `strlen` and string reversal, there should only be one pass of the string sequence. There will be a stateless consumption function that takes in a token (`t`), the index (`i`), and an accumulator (`n`). To transform `t` to the decimal number that it is supposed to represent, a mask of `0x30` will be applied using a bitwise XOR. To speed up the calculation of the mulitplied sum digit, a static lookup table will be provided for all possible values in range [0, 9].

### Lookup table

Calculation: `sumdigit(2n)`

```txt
+--------+---+---+---+---+---+---+---+---+---+---+
| Input  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
+--------+---+---+---+---+---+---+---+---+---+---+
| Output | 0 | 2 | 4 | 6 | 8 | 1 | 3 | 5 | 7 | 9 |
+--------+---+---+---+---+---+---+---+---+---+---+
```

Lookup is simple since the decimal of `t` can be used as the index so we can store the table in a single array instead of a map.

### Reference implementation

```py
arr = [0, 2, 4, 6, 8, 1, 3, 5, 7, 9]

test = "17893729974"

check, *rest = reversed(test)

n = 0
for i, t in enumerate(rest):
  n += arr[int(t)] if i % 2 == 0 else int(t)

assert 10 - (n % 10) == int(check)
```
