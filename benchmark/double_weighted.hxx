// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DOUBLE_WEIGHTED
#define DOUBLE_WEIGHTED

#include <EPHist/EPHist.hxx>

#include <benchmark/benchmark.h>

#include <random>
#include <vector>

struct DoubleWeighted : public benchmark::Fixture {
  // The histogram is stored and constructed in the fixture to avoid compiler
  // optimizations in the benchmark body taking advantage of the (constant)
  // constructor parameters.
  EPHist::EPHist<double> h1{20, 0.0, 1.0};
  std::vector<double> fNumbers;

  void SetUp(benchmark::State &state) {
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
    fNumbers.resize(2 * state.range(0));
    for (std::size_t i = 0; i < fNumbers.size(); i++) {
      fNumbers[i] = dis(gen);
    }
  }
};

#endif
