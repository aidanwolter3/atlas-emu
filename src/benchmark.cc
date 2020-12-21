#include <benchmark/benchmark.h>

#include "src/atlas.h"

// The CPU should run at 1.78 MHzs, therefore on a good day this will run for
// exactly 5.59 ms.
static void BM_100_NOPs(benchmark::State& state) {
  for (auto _ : state) {
    Atlas atlas("src/roms/100_nops.nes", /*headless=*/true);
    atlas.Run();
  }
}
BENCHMARK(BM_100_NOPs)->Unit(benchmark::kMillisecond);
BENCHMARK_MAIN();
