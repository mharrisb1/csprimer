import struct
import time
import random

import cvarint


def encode(n):
    out = []
    while n > 0:
        part = n & 0x7f
        n >>= 7
        part |= (n and 0x80 or 0x00)
        out.append(part)
    return bytes(out)


def decode(varn):
    n = 0
    for b in reversed(varn):
        n <<= 7
        n |= (b & 0x7f)
    return n


if __name__ == '__main__':
    # Basic cases to ensure encoding is correct
    cases = (
        ('1.uint64', b'\x01'),
        ('150.uint64', b'\x96\x01'),
        ('maxint.uint64', b'\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01'),
    )
    for fname, expectation in cases:
        with open(fname, 'rb') as f:
            n = struct.unpack('>Q', f.read())[0]
            assert encode(n) == expectation
            assert decode(encode(n)) == n
            assert cvarint.encode(n) == expectation
            assert cvarint.decode(cvarint.encode(n)) == n

    # Now roundtrip test for speed!
    num_cases = 1000000
    py_time = 0
    c_time = 0

    print('Running speed test...')

    for _ in range(num_cases):
        n = random.getrandbits(64)
        # time the python
        start = time.process_time_ns()
        res = decode(encode(n))
        end = time.process_time_ns()
        py_time += (end - start)
        assert n == res
        # time the C
        start = time.process_time_ns()
        res = cvarint.decode(cvarint.encode(n))
        end = time.process_time_ns()
        c_time += (end - start)
        try:
            assert n == res
        except AssertionError:
            print(f'Failed on {n}: round trip result was {res}')
            break

    py_time_sec = float(py_time) / 1000000000
    c_time_sec = float(c_time) / 1000000000
    print(f'Executed {num_cases:,} random tests\n\n'
          f'Python:\t{int(py_time/num_cases):>6}ns per case '
          f'({py_time_sec:0.3f}s total)\n'
          f'C:\t{int(c_time/num_cases):>6}ns per case '
          f'({c_time_sec:0.3f}s total)')
