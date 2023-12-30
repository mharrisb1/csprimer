# Varint Module Notes

## Python solution

```sh
cat 1.uint64 150.uint64 maxint.uint64 | python3 varint.py
Base 128 Varints

✅ decoded:                    1    encoded: b'\x01'
✅ decoded:                  150    encoded: b'\x96\x01'
✅ decoded: 18446744073709551615    encoded: b'\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01'
```

## Rust solution

```sh
cd rust-solution
cargo test

running 2 tests
test tests::test_decode ... ok
test tests::test_encode ... ok
```
