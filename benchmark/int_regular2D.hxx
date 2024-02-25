// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef INT_REGULAR_2D
#define INT_REGULAR_2D

#include <EPHist/EPHist.hxx>

#include <benchmark/benchmark.h>

#include <random>
#include <vector>

struct IntRegular2D : public benchmark::Fixture {
  // The histogram is stored and constructed in the fixture to avoid compiler
  // optimizations in the benchmark body taking advantage of the (constant)
  // constructor parameters.
  EPHist::RegularAxis axis{20, 0.0, 1.0};
  EPHist::EPHist<int> h2{{axis, axis}};
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
