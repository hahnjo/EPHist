// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndexRange.hxx>
#include <EPHist/EPHist.hxx>

#include <benchmark/benchmark.h>

static void IntRegular2D_Slice(benchmark::State &state) {
  EPHist::RegularAxis axis{20, 0.0, 1.0};
  EPHist::EPHist<int> h2{{axis, axis}};
  auto range = EPHist::BinIndexRange(5, 15);
  for (auto _ : state) {
    benchmark::DoNotOptimize(h2.Slice(range, range));
  }
}
BENCHMARK(IntRegular2D_Slice);

#ifndef ALL_BENCHMARKS
BENCHMARK_MAIN();
#endif
