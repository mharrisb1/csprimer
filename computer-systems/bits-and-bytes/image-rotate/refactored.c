#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HEADER_SIZE         54
#define FILE_HEADER__OFFSET 10
#define DIB_HEADER__WIDTH   18
#define DIB_HEADER__HEIGHT  22
#define DIB_HEADER__BPP     28

int main(int argc, char **argv) {
  FILE *instream, *outstream;
  uint8_t header[HEADER_SIZE];

  instream  = fopen(argv[1], "rb");
  outstream = fopen(argv[2], "wb");

  fread(header, 1, HEADER_SIZE, instream);

  uint32_t offset = *(uint32_t *)&header[FILE_HEADER__OFFSET];
  uint32_t width  = *(int32_t *)&header[DIB_HEADER__WIDTH];
  uint32_t height = *(int32_t *)&header[DIB_HEADER__HEIGHT];
  uint8_t depth   = *(uint16_t *)&header[DIB_HEADER__BPP] >> 3;

  float rad       = atol(argv[3]) * (M_PI / 180.0f);
  float cos_theta = cos(rad);
  float sin_theta = sin(rad);
  int64_t volume  = height * width * depth;

  uint8_t *readbuf  = malloc(volume);
  uint8_t *writebuf = calloc(volume, 1);

  fseek(instream, offset, SEEK_SET);
  fread(readbuf, 1, volume, instream);

  int32_t norm_x = width >> 1;
  int32_t norm_y = height >> 1;

  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      int32_t rel_y = y - norm_y;
      int32_t rel_x = x - norm_x;

      int32_t y_hat = (rel_y * cos_theta - rel_x * sin_theta) + norm_y;
      int32_t x_hat = (rel_x * cos_theta + rel_y * sin_theta) + norm_x;

      if (y_hat >= 0 && y_hat < height && x_hat >= 0 && x_hat < width) {
        uint32_t dst = (y * width + x) * depth;

        for (uint8_t i = 0; i < depth; i++) {

          int32_t x1 = (int32_t)floor(x_hat);
          int32_t y1 = (int32_t)floor(y_hat);
          int32_t x2 = x1 + 1;
          int32_t y2 = y1 + 1;

          if (x2 >= width)
            x2 = width - 1;
          if (y2 >= height)
            y2 = height - 1;

          float a = x_hat - x1;
          float b = y_hat - y1;

          uint32_t p1 = (y1 * width + x1) * depth + i;
          uint32_t p2 = (y1 * width + x2) * depth + i;
          uint32_t p3 = (y2 * width + x1) * depth + i;
          uint32_t p4 = (y2 * width + x2) * depth + i;

          writebuf[dst + i] =
              (uint8_t)((1 - a) * (1 - b) * readbuf[p1] +
                        a * (1 - b) * readbuf[p2] + (1 - a) * b * readbuf[p3] +
                        a * b * readbuf[p4]);
        }
      }
    }
  }

  fwrite(header, 1, HEADER_SIZE, outstream);
  fseek(outstream, offset, SEEK_SET);
  fwrite(writebuf, 1, volume, outstream);

  free(readbuf);
  free(writebuf);
  fclose(instream);
  fclose(outstream);
}
