// SPDX-License-Identifier: LGPL-3.0-or-later

#include "int_regular1D.hxx"

#include <EPHist/EPHist.hxx>

#include <benchmark/benchmark.h>

BENCHMARK_DEFINE_F(IntRegular1D, TemplatedFillAtomic)(benchmark::State &state) {
  for (auto _ : state) {
    for (double number : fNumbers) {
      h1.FillAtomic<EPHist::RegularAxis>(number);
    }
    h1.Clear();
  }
}
BENCHMARK_REGISTER_F(IntRegular1D, TemplatedFillAtomic)->Range(0, 32768);

#ifndef ALL_BENCHMARKS
BENCHMARK_MAIN();
#endif
