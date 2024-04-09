#define RED_MASK 0xE0
#define GREEN_MASK 0x1C
#define BLUE_MASK 0x03

unsigned char quantize(unsigned char red, unsigned char green,
                       unsigned char blue) {
  unsigned char out = 0;
  out += red & RED_MASK;
  out += (green >> 3) & GREEN_MASK;
  out += (blue >> 6) & BLUE_MASK;

  return out;
}
