#define MASK 0xE0

unsigned char quantize(unsigned char red, unsigned char green,
                       unsigned char blue) {
  return (red & MASK) + (green & MASK >> 3) + (blue & MASK >> 6);
}
