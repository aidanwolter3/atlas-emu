#include <benchmark/benchmark.h>

#include "src/atlas.h"

// Times how long it takes to load a ROM into memory by simply constructing the
// Atlas class.
static void BM_LoadROM(benchmark::State& state) {
  for (auto _ : state) {
    Atlas atlas("src/roms/100_nops.nes", /*headless=*/true);
  }
}
BENCHMARK(BM_LoadROM)->Unit(benchmark::kMillisecond);

// The CPU should run at 1.78 MHzs, therefore on a good day this will run for
// exactly 5.59 us.
static void BM_100_NOPs_in_6us(benchmark::State& state) {
  Atlas atlas("src/roms/100_nops.nes", /*headless=*/true);
  for (auto _ : state) {
    atlas.Run();
    atlas.Reset();  // affects the time, unfortunately
  }
}
BENCHMARK(BM_100_NOPs_in_6us)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
