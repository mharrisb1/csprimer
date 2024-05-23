#include <stdio.h>

#define BMP_HEADER__HEADER_FIELD 0
#define BMP_HEADER__STARTING_ADDRESS 10
#define DIB_HEADER__HEADER_SIZE 14
#define DIB_HEADER__PIXEL_WIDTH 18
#define DIB_HEADER__PIXEL_HEIGHT 22

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  const char *fname = argv[1];
}

/*

   +----------+----------+
10 |          |          |
8  |          x          |
6  |        w | z        |
4  |      y   |   y      |
2  |    z     |     w    |
0  +--x-------+-------x--+
   |    w     |     z    |
   |      y   |   y      |
   |        z | w        |
   |          x          |
   |          |          |
   +----------+----------+
  -10987654321012345678910

  x0 = ( 0,  8) = x(360deg)
  x1 = ( 8,  0)
  x2 = ( 0, -8)
  x3 = (-8,  0)

  y0 = ( 4,  4) = x(45deg )
  y1 = ( 4, -4) = x(135deg)
  y2 = (-4, -4) = x(225deg)
  y3 = (-4,  4) = x(315deg)

  z0 = ( 2,  6)
  z1 = ( 6, -2)
  z2 = (-2, -6)
  z3 = (-6,  2)

  w0 = ( 6,  2)
  w1 = ( 2, -6)
  w2 = (-6, -2)
  w3 = (-2,  6)

  Table:

  | x | y | deg |
  |---|---|-----|
  | 0 | 8 |   0 |
  | 4 | 4 |  45 |
  | 8 | 0 |  90 |
  | 4 |-4 | 135 |
  | 0 |-8 | 180 |
  |-4 |-4 | 225 |
  |-8 | 0 | 270 |
  |-4 | 4 | 315 |
  | 0 | 8 | 360 |

  Equation:

  $$
    p' = f(p, d)

    f(p, d) = f((x, y), d)

    x = ((180 - (deg % 360)) / 90) * y
  $$

*/
