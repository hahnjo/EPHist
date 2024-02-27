#define ALL_BENCHMARKS
#include "int_regular1D_Fill.cxx"
#include "int_regular1D_Fill_tuple.cxx"
#include "int_regular1D_templated_Fill.cxx"
#include "int_regular2D_Fill.cxx"
#include "int_regular2D_Fill_tuple.cxx"
#include "int_regular2D_templated_Fill.cxx"

#include "double_regular1D_Fill.cxx"
#include "double_regular1D_Fill_tuple.cxx"
#include "double_regular1D_templated_Fill.cxx"

#include <benchmark/benchmark.h>

BENCHMARK_MAIN();
