// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndexRange.hxx>
#include <EPHist/EPHist.hxx>

#include <benchmark/benchmark.h>

static void IntRegular1D_Slice(benchmark::State &state) {
  EPHist::EPHist<int> h1{20, 0.0, 1.0};
  auto range = EPHist::BinIndexRange(5, 15);
  for (auto _ : state) {
    benchmark::DoNotOptimize(h1.Slice(range));
  }
}
BENCHMARK(IntRegular1D_Slice);

#ifndef ALL_BENCHMARKS
BENCHMARK_MAIN();
#endif
