import sys
import tty
import termios
from contextlib import contextmanager


@contextmanager
def terminal():
    tty_attrs = termios.tcgetattr(sys.stdin)
    try:
        tty.setcbreak(sys.stdin)
        yield
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, tty_attrs)


if __name__ == "__main__":
    with terminal():
        try:
            while True:
                ch = ord(sys.stdin.buffer.read(1))
                if 0x31 <= ch <= 0x39:
                    for _ in range(ch - 0x30):
                        sys.stdout.buffer.write(b"\x07")
                sys.stdout.buffer.flush()
        except KeyboardInterrupt:
            pass
