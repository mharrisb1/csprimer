# Varint Module Notes

## Protocol Buffers Encoding
- 

## Misc

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
