#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BMP_HEADER__STARTING_ADDRESS 10
#define DIB_HEADER__PIXEL_WIDTH 18
#define DIB_HEADER__PIXEL_HEIGHT 22
#define DIB_HEADER__BP_PIXEL 28

void rotate(int16_t x1, int16_t y1, int16_t deg, int16_t *x2, int16_t *y2) {
  float rad = deg * (M_PI / 180.0);
  float cos_theta = cos(rad);
  float sin_theta = sin(rad);

  *x2 = (int16_t)(x1 * cos_theta - y1 * sin_theta);
  *y2 = (int16_t)(x1 * sin_theta + y1 * cos_theta);
}

int main(int argc, char **argv) {
  const char *infname = argv[1];
  const char *outfname = argv[2];
  const int16_t deg = atol(argv[3]);

  FILE *infile = fopen(infname, "rb");
  FILE *outfile = fopen(outfname, "wb");

  // read header data
  uint8_t bmp_header[54];
  fread(bmp_header, 1, 54, infile); // Read the BMP header

  int32_t width = *(int32_t *)&bmp_header[DIB_HEADER__PIXEL_WIDTH];
  int32_t height = *(int32_t *)&bmp_header[DIB_HEADER__PIXEL_HEIGHT];
  uint16_t bits_per_pixel = *(uint16_t *)&bmp_header[DIB_HEADER__BP_PIXEL];
  uint32_t offset = *(uint32_t *)&bmp_header[BMP_HEADER__STARTING_ADDRESS];

  // calculate sizes
  uint8_t bytes_per_pixel = bits_per_pixel / 8;
  size_t pixel_array_size = width * height * bytes_per_pixel;
  size_t write_size = offset + pixel_array_size;

  // allocate buffers
  uint8_t *read_buffer = (uint8_t *)malloc(pixel_array_size);
  uint8_t *write_buffer = (uint8_t *)calloc(pixel_array_size, sizeof(uint8_t));

  // read pixel data
  fseek(infile, offset, SEEK_SET);
  fread(read_buffer, 1, pixel_array_size, infile);

  // rotate pixel array and store in write buffer
  int16_t row_t, col_t;
  for (int16_t row = 0; row < height; row++) {
    for (int16_t col = 0; col < width; col++) {
      int32_t src_index = (row * width + col) * bytes_per_pixel;
      rotate(row - height / 2, col - width / 2, deg, &row_t, &col_t);
      row_t += height / 2;
      col_t += width / 2;

      if (row_t >= 0 && row_t < height && col_t >= 0 && col_t < width) {
        int32_t dest_index = (row_t * width + col_t) * bytes_per_pixel;
        memcpy(write_buffer + dest_index, read_buffer + src_index,
               bytes_per_pixel);
      }
    }
  }

  // write headers and rotated pixel array to output file
  fwrite(bmp_header, 1, 54, outfile); // Write BMP header
  fseek(outfile, offset, SEEK_SET);
  fwrite(write_buffer, 1, pixel_array_size, outfile);

  // Clean up
  free(read_buffer);
  free(write_buffer);
  fclose(infile);
  fclose(outfile);

  return 0;
}
