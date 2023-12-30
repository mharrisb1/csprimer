import sys
import struct


def encode(n: int) -> bytes:
    out = bytearray()  # https://youtu.be/z9Hmys8ojno?si=VvmI69QNoSh2MndJ
    while n:
        part = n & 0x7F
        n >>= 7
        out.append(part | 0x80 if n else part)

    return bytes(out)


def decode(varn: bytes) -> int:
    n = 0
    for b in reversed(varn):
        n <<= 7
        n |= b & 0x7F

    return n


if __name__ == "__main__":
    print("Base 128 Varints\n")
    while True:
        chunk = sys.stdin.buffer.read(8)
        if not chunk:
            break

        unpacked = struct.unpack(">Q", chunk)[0]
        encoded = encode(unpacked)
        decoded = decode(encoded)
        status = "✅" if decoded == unpacked else "❌"
        print(f"{status} decoded: {decoded:20d}\tencoded: {encoded}")
