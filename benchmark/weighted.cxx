#define ALL_BENCHMARKS
#include "double_weighted_Fill.cxx"
#include "double_weighted_Fill_tuple.cxx"
#include "double_weighted_templated_Fill.cxx"

#include "DoubleBinWithError_weighted_Fill.cxx"
#include "DoubleBinWithError_weighted_Fill_tuple.cxx"
#include "DoubleBinWithError_weighted_templated_Fill.cxx"

#include <benchmark/benchmark.h>

BENCHMARK_MAIN();
