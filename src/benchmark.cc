#include <benchmark/benchmark.h>

#include "src/atlas.h"

// Times how long it takes to load a ROM into memory by simply constructing the
// Atlas class.
static void BM_Load_ROM(benchmark::State& state) {
  for (auto _ : state) {
    Atlas atlas("src/roms/100_nops.nes", /*headless=*/true);
  }
}
BENCHMARK(BM_Load_ROM)->Unit(benchmark::kMicrosecond);

// The CPU should run at 1.79 MHzs, therefore on a good day this will run for
// exactly 55.9 us.
static void BM_100_NOPs_in_56us(benchmark::State& state) {
  Atlas atlas("src/roms/100_nops.nes", /*headless=*/true);
  for (auto _ : state) {
    atlas.Run();
    atlas.Reset();  // affects the time, unfortunately
  }
}
BENCHMARK(BM_100_NOPs_in_56us)->Unit(benchmark::kMicrosecond);

// The CPU should run at 1.79 MHzs, therefore on a good day this will run for
// exactly 55.9 us.
static void BM_CPU_Reset(benchmark::State& state) {
  Atlas atlas("src/roms/100_nops.nes", /*headless=*/true);
  for (auto _ : state) {
    atlas.Reset();
  }
}
BENCHMARK(BM_CPU_Reset);

BENCHMARK_MAIN();
