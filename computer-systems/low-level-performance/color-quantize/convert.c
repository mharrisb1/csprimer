#include <stdio.h>
#include <stdlib.h>

#define BPP_OUT 8 // output bits per pixel
#define int32le(b) ((b)[0] | ((b)[1] << 8) | ((b)[2] << 16) | ((b)[3] << 24))
#define row_width(w, bpp)                                                      \
  ((((w) * (bpp) + 31) >> 5) << 2) // round row width up to multiple of 4

extern unsigned char quantize(unsigned char red, unsigned char green,
                              unsigned char blue);

void convert(unsigned char *in, unsigned char *out, int32_t width,
             int32_t height) {
  // *in is the raw pixels of the bitmap, 24 bits per pixel in RGB order
  // *out is an array of 8 bit indexes into our color table
  int x, y, i_in, i_out;
  int row_width_in = row_width(width, 24);
  int row_width_out = row_width(width, 8);
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      i_in = row_width_in * y + 3 * x;
      i_out = row_width_out * y + x;
      out[i_out] = quantize(in[i_in + 2], in[i_in + 1], in[i_in]);
    }
  }
}

struct rgba {
  unsigned char b, g, r, a;
};

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s in.bmp out.bmp\n", argv[0]);
    exit(1);
  }

  // open source file and read bytes
  FILE *fh = fopen(argv[1], "r");
  fseek(fh, 0, SEEK_END);
  long size = ftell(fh);
  rewind(fh);
  unsigned char *bytes_in = malloc(size);
  fread(bytes_in, size, 1, fh);
  fclose(fh);
  int32_t offset = int32le(bytes_in + 10), width = int32le(bytes_in + 18),
          height = int32le(bytes_in + 22);

  unsigned char *colors_out = malloc(row_width(width, BPP_OUT) * height);

  convert(bytes_in + offset, colors_out, width, height);
  // write output
  fh = fopen(argv[2], "w");
  struct rgba color_table[256];
  // origin header, modified in place
  int new_offset = offset + sizeof(color_table);
  bytes_in[10] = new_offset % 256;
  bytes_in[11] = new_offset / 256;
  bytes_in[28] = 8; // 8 bits per pixel
  bytes_in[29] = 0;
  bytes_in[34] = 0; // compute new size automatically
  bytes_in[35] = 0;
  bytes_in[36] = 0;
  bytes_in[37] = 0;
  bytes_in[46] = 0;
  bytes_in[47] = 1; // 256 colors in color table

  fwrite(bytes_in, offset, 1, fh);
  // build and write color table
  unsigned char rgs[] = {0x00, 0x24, 0x49, 0x6d, 0x92, 0xb6, 0xdb, 0xff};
  unsigned char bs[] = {0x00, 0x55, 0xaa, 0xff};
  for (int i = 0, ri = 0; ri < 8; ri++)
    for (int gi = 0; gi < 8; gi++)
      for (int bi = 0; bi < 4; bi++, i++)
        color_table[i] = (struct rgba){bs[bi], rgs[gi], rgs[ri], 0x00};

  fwrite(color_table, sizeof(color_table), 1, fh);
  // new color indexes
  fwrite(colors_out, row_width(width, BPP_OUT) * height, 1, fh);
  fclose(fh);
  free(bytes_in);
  free(colors_out);
}
