# Varint Module Notes

## Links

1. [Base 128 varints](https://protobuf.dev/programming-guides/encoding/#varints)

## Explainers

### Signed vs. Unsigned Integers

At a simplfied level, we can say that:

- unsigned integers contains the set of [natural numbers](https://en.wikipedia.org/wiki/Natural_number) ($\mathbb{N}$)
- signed integers contains the set of [integers](https://en.wikipedia.org/wiki/Integer) ($\mathbb{Z}$)

Because computers need to work with discrete chunks of data we need to usually[^1] deal with sized integers
so for example a 4-bit unsigned integer (not usually something that is supported) could represent any natural
number from [0, 15]. For example, you would have something like `0101` which would represent 5.

When dealing with *signed* integers, the most significant bit will either be a 0 for positive integers or 1 for
negative integers. So, again if we used a size of 4-bits we could represent any number from [-7, 7]. For example,
if you had something like `1001`, that would be equal to -1.

[^1]: Some programming languages like Python support variable size integers.

### What does it mean for a value to be a certain number of bit?

- For a lot of high-level languages like Python and JS, you don't have to think about the size of data types like integers
- In lower-level languages (C/C++, Rust, C#, Java, Go) you do need to think about this becuase of things like overflows/underflows, efficiency, etc.

Example:

Let's say we have a 4-bit integer (unsigned for simplicity)

### Little vs. Big Endian

[Source](https://www.youtube.com/watch?v=jhErugDB-34)

- **Byte ordering**: how multi-byte types are stored in memory
- Computer memory is just a large array of bytes (8 bits)
- There are 2 different standards to byte ordering:
  1. Little endian
  2. Big endian

Example:

32-bit (4-byte) integer: `12 34 56 78` (305419896)

We can store it in big endian:

| Address | Contents (Hex) |
| ------- | -------------- |
| 100     | 12             |
| 101     | 34             |
| 102     | 56             |
| 103     | 78             |

Or in little endian:

| Address | Contents (Hex) |
| ------- | -------------- |
| 100     | 78             |
| 101     | 56             |
| 102     | 34             |
| 103     | 12             |
