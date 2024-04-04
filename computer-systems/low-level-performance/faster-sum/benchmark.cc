#include <benchmark/benchmark.h>

#define NUM_ELEMENTS (1 << 20)

extern "C" int sum(int *nums, int n);

int *nums;

static void DoSetup(const benchmark::State &state) {
  nums = (int *)malloc(NUM_ELEMENTS * sizeof(int));
  srand(1);
  for (int i = 0; i < NUM_ELEMENTS; i++)
    nums[i] = rand();
}

static void DoTeardown(const benchmark::State &state) { free(nums); }

static void BM_Sum(benchmark::State &state) {
  for (auto _ : state) {
    int total = sum(nums, state.range(0));
    benchmark::DoNotOptimize(total);
  }
}

BENCHMARK(BM_Sum)
    ->Unit(benchmark::kMicrosecond)
    ->RangeMultiplier(4)
    ->Range(1 << 12, NUM_ELEMENTS)
    ->Setup(DoSetup)
    ->Teardown(DoTeardown);

BENCHMARK_MAIN();
