// SPDX-License-Identifier: LGPL-3.0-or-later

#include "int_regular2D.hxx"

#include <EPHist/EPHist.hxx>

#include <benchmark/benchmark.h>

#include <tuple>

BENCHMARK_DEFINE_F(IntRegular2D, FillTuple)(benchmark::State &state) {
  for (auto _ : state) {
    for (std::size_t i = 0; i < state.range(0); i++) {
      h2.Fill(std::make_tuple(fNumbers[2 * i], fNumbers[2 * i + 1]));
    }
    h2.Clear();
  }
}
BENCHMARK_REGISTER_F(IntRegular2D, FillTuple)->Range(0, 32768);

#ifndef ALL_BENCHMARKS
BENCHMARK_MAIN();
#endif
