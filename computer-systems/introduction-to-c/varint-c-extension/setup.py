from setuptools import setup, Extension


def main():
    setup(name="cvarint",
          version="1.0.0",
          description="Varint encoding and decoding in C",
          ext_modules=[Extension("cvarint", ["cvarintmodule.c"])])


if __name__ == '__main__':
    main()
