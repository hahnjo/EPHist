find_package(benchmark QUIET)
if(NOT benchmark_FOUND)
  message(WARNING "benchmark not found, disabling benchmarks")
  return()
endif()

add_executable(benchmark_regular regular.cxx)
target_link_libraries(benchmark_regular EPHist benchmark::benchmark)

add_executable(benchmark_int_regular1D_Fill int_regular1D_Fill.cxx)
target_link_libraries(benchmark_int_regular1D_Fill EPHist benchmark::benchmark)

add_executable(benchmark_int_regular2D_Fill int_regular2D_Fill.cxx)
target_link_libraries(benchmark_int_regular2D_Fill EPHist benchmark::benchmark)

add_executable(benchmark_double_regular1D_Fill double_regular1D_Fill.cxx)
target_link_libraries(benchmark_double_regular1D_Fill EPHist benchmark::benchmark)
