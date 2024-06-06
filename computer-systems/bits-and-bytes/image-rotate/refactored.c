#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HEADER_SIZE         54
#define FILE_HEADER__OFFSET 10
#define DIB_HEADER__WIDTH   18
#define DIB_HEADER__HEIGHT  22
#define DIB_HEADER__BPP     28

float lerp(float a, float b, float t) { return a + t * (b - a); }

float bilerp(float tx, float ty, float tl, float tr, float bl, float br) {
  float A = lerp(tl, tr, tx);
  float B = lerp(bl, br, tx);
  return lerp(A, B, ty);
}

void rotate(uint8_t *readbuf, uint8_t *writebuf, uint32_t height,
            uint32_t width, uint8_t depth, int32_t deg) {
  float rad       = deg * (M_PI / 180.0f);
  float cos_theta = cos(rad);
  float sin_theta = sin(rad);

  int32_t norm_x = width >> 1;
  int32_t norm_y = height >> 1;

  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      int32_t rel_y = y - norm_y;
      int32_t rel_x = x - norm_x;

      float y_hat = (rel_y * cos_theta - rel_x * sin_theta) + norm_y;
      float x_hat = (rel_x * cos_theta + rel_y * sin_theta) + norm_x;

      if (y_hat >= 0 && y_hat < height && x_hat >= 0 && x_hat < width) {
        uint32_t target = (y * width + x) * depth;

        uint32_t x1 = floorf(x_hat);
        uint32_t y1 = floorf(y_hat);
        uint32_t x2 = x1 + 1;
        uint32_t y2 = y1 + 1;

        if (x2 >= width) {
          x2 = width - 1;
        }
        if (y2 >= height) {
          y2 = height - 1;
        }

        float tx = x_hat - x1;
        float ty = y_hat - y1;

        for (uint8_t channel = 0; channel < depth; channel++) {
          float tl = readbuf[(y1 * width + x1) * depth + channel];
          float tr = readbuf[(y1 * width + x2) * depth + channel];
          float bl = readbuf[(y2 * width + x1) * depth + channel];
          float br = readbuf[(y2 * width + x2) * depth + channel];

          writebuf[target + channel] = (uint8_t)bilerp(tx, ty, tl, tr, bl, br);
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  FILE *instream, *outstream;
  uint8_t header[HEADER_SIZE];

  instream  = fopen(argv[1], "rb");
  outstream = fopen(argv[2], "wb");

  uint32_t deg = atol(argv[3]);

  fread(header, 1, HEADER_SIZE, instream);

  uint32_t offset = *(uint32_t *)&header[FILE_HEADER__OFFSET];
  uint32_t width  = *(int32_t *)&header[DIB_HEADER__WIDTH];
  uint32_t height = *(int32_t *)&header[DIB_HEADER__HEIGHT];
  uint8_t depth   = *(uint16_t *)&header[DIB_HEADER__BPP] >> 3;
  int64_t volume  = height * width * depth;

  uint8_t *readbuf  = malloc(volume);
  uint8_t *writebuf = calloc(volume, 1);

  fseek(instream, offset, SEEK_SET);
  fread(readbuf, 1, volume, instream);

  rotate(readbuf, writebuf, height, width, depth, deg);

  fwrite(header, 1, HEADER_SIZE, outstream);
  fseek(outstream, offset, SEEK_SET);
  fwrite(writebuf, 1, volume, outstream);

  free(readbuf);
  free(writebuf);
  fclose(instream);
  fclose(outstream);
}
