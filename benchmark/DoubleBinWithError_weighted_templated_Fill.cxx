// SPDX-License-Identifier: LGPL-3.0-or-later

#include "DoubleBinWithError_weighted.hxx"

#include <EPHist/EPHist.hxx>
#include <EPHist/Weight.hxx>

#include <benchmark/benchmark.h>

BENCHMARK_DEFINE_F(DoubleBinWithErrorWeighted, TemplatedFill)
(benchmark::State &state) {
  for (auto _ : state) {
    for (std::size_t i = 0; i < state.range(0); i++) {
      h1.Fill<EPHist::RegularAxis>(EPHist::Weight(fNumbers[2 * i]),
                                   fNumbers[2 * i + 1]);
    }
    h1.Clear();
  }
}
BENCHMARK_REGISTER_F(DoubleBinWithErrorWeighted, TemplatedFill)
    ->Range(0, 32768);

#ifndef ALL_BENCHMARKS
BENCHMARK_MAIN();
#endif
