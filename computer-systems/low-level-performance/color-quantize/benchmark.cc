#include <benchmark/benchmark.h>

#define NUM_PIXELS (1 << 22)

extern "C" unsigned char quantize(unsigned char red, unsigned char green,
                                  unsigned char blue);

unsigned char *pixels;

static void DoSetup(const benchmark::State &state) {
  pixels = (unsigned char *)malloc(NUM_PIXELS * 3 * sizeof(unsigned char));
  srand(1);
  for (int i = 0; i < 3 * NUM_PIXELS; i += 3) {
    pixels[i] = rand() & 0xff;
    pixels[i + 1] = rand() & 0xff;
    pixels[i + 2] = rand() & 0xff;
  }
}

static void DoTeardown(const benchmark::State &state) { free(pixels); }

static void BM_quantize(benchmark::State &state) {
  for (auto _ : state) {
    for (int i = 0; i < 3 * NUM_PIXELS; i += 3) {
      benchmark::DoNotOptimize(
          quantize(pixels[i], pixels[i + 1], pixels[i + 2]));
    }
  }
}

BENCHMARK(BM_quantize)
    ->Unit(benchmark::kMillisecond)
    ->Setup(DoSetup)
    ->Teardown(DoTeardown);

BENCHMARK_MAIN();

