#define ALL_BENCHMARKS
#include "int_regular1D_FillAtomic.cxx"
#include "int_regular1D_templated_FillAtomic.cxx"
#include "int_regular2D_FillAtomic.cxx"
#include "int_regular2D_templated_FillAtomic.cxx"

#include "double_regular1D_FillAtomic.cxx"
#include "double_regular1D_templated_FillAtomic.cxx"

#include <benchmark/benchmark.h>

BENCHMARK_MAIN();
