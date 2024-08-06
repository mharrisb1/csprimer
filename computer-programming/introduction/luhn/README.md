# Luhn algorithm

Implementations of the Luhn algorithm in 4 different languages:

1. Scheme ([Steel](https://github.com/mattwparas/steel) dialect)
2. C
3. A64 Assembly (🚧 incomplete)
4. Python (used as reference below)

## Reference implementation

```py
arr = [0, 2, 4, 6, 8, 1, 3, 5, 7, 9]

test = "17893729974"

check, *rest = reversed(test)

n = 0
for i, t in enumerate(rest):
  n += arr[int(t)] if i % 2 == 0 else int(t)

assert 10 - (n % 10) == int(check)
```
