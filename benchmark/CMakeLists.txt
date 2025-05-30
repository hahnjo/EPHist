find_package(Threads REQUIRED)
add_executable(benchmark_mt_int_regular1D mt_int_regular1D.cxx)
target_link_libraries(benchmark_mt_int_regular1D EPHist Threads::Threads)

find_package(benchmark QUIET)
if(NOT benchmark_FOUND)
  message(WARNING "benchmark not found, disabling most benchmarks")
  return()
endif()

add_executable(benchmark_atomic atomic.cxx)
target_link_libraries(benchmark_atomic EPHist benchmark::benchmark)

add_executable(benchmark_regular regular.cxx)
target_link_libraries(benchmark_regular EPHist benchmark::benchmark)

add_executable(benchmark_int_regular1D_Fill int_regular1D_Fill.cxx)
target_link_libraries(benchmark_int_regular1D_Fill EPHist benchmark::benchmark)
add_executable(benchmark_int_regular1D_FillAtomic int_regular1D_FillAtomic.cxx)
target_link_libraries(benchmark_int_regular1D_FillAtomic EPHist benchmark::benchmark)
add_executable(benchmark_int_regular1D_Fill_tuple int_regular1D_Fill_tuple.cxx)
target_link_libraries(benchmark_int_regular1D_Fill_tuple EPHist benchmark::benchmark)
add_executable(benchmark_int_regular1D_templated_Fill int_regular1D_templated_Fill.cxx)
target_link_libraries(benchmark_int_regular1D_templated_Fill EPHist benchmark::benchmark)
add_executable(benchmark_int_regular1D_templated_FillAtomic int_regular1D_templated_FillAtomic.cxx)
target_link_libraries(benchmark_int_regular1D_templated_FillAtomic EPHist benchmark::benchmark)
add_executable(benchmark_int_regular1D_Slice int_regular1D_Slice.cxx)
target_link_libraries(benchmark_int_regular1D_Slice EPHist benchmark::benchmark)

add_executable(benchmark_int_regular2D_Fill int_regular2D_Fill.cxx)
target_link_libraries(benchmark_int_regular2D_Fill EPHist benchmark::benchmark)
add_executable(benchmark_int_regular2D_FillAtomic int_regular2D_FillAtomic.cxx)
target_link_libraries(benchmark_int_regular2D_FillAtomic EPHist benchmark::benchmark)
add_executable(benchmark_int_regular2D_Fill_tuple int_regular2D_Fill_tuple.cxx)
target_link_libraries(benchmark_int_regular2D_Fill_tuple EPHist benchmark::benchmark)
add_executable(benchmark_int_regular2D_templated_Fill int_regular2D_templated_Fill.cxx)
target_link_libraries(benchmark_int_regular2D_templated_Fill EPHist benchmark::benchmark)
add_executable(benchmark_int_regular2D_templated_FillAtomic int_regular2D_templated_FillAtomic.cxx)
target_link_libraries(benchmark_int_regular2D_templated_FillAtomic EPHist benchmark::benchmark)
add_executable(benchmark_int_regular2D_Slice int_regular2D_Slice.cxx)
target_link_libraries(benchmark_int_regular2D_Slice EPHist benchmark::benchmark)

add_executable(benchmark_double_regular1D_Fill double_regular1D_Fill.cxx)
target_link_libraries(benchmark_double_regular1D_Fill EPHist benchmark::benchmark)
add_executable(benchmark_double_regular1D_FillAtomic double_regular1D_FillAtomic.cxx)
target_link_libraries(benchmark_double_regular1D_FillAtomic EPHist benchmark::benchmark)
add_executable(benchmark_double_regular1D_Fill_tuple double_regular1D_Fill_tuple.cxx)
target_link_libraries(benchmark_double_regular1D_Fill_tuple EPHist benchmark::benchmark)
add_executable(benchmark_double_regular1D_templated_Fill double_regular1D_templated_Fill.cxx)
target_link_libraries(benchmark_double_regular1D_templated_Fill EPHist benchmark::benchmark)
add_executable(benchmark_double_regular1D_templated_FillAtomic double_regular1D_templated_FillAtomic.cxx)
target_link_libraries(benchmark_double_regular1D_templated_FillAtomic EPHist benchmark::benchmark)

add_executable(benchmark_weighted weighted.cxx)
target_link_libraries(benchmark_weighted EPHist benchmark::benchmark)

add_executable(benchmark_double_weighted_Fill double_weighted_Fill.cxx)
target_link_libraries(benchmark_double_weighted_Fill EPHist benchmark::benchmark)
add_executable(benchmark_double_weighted_Fill_tuple double_weighted_Fill_tuple.cxx)
target_link_libraries(benchmark_double_weighted_Fill_tuple EPHist benchmark::benchmark)
add_executable(benchmark_double_weighted_templated_Fill double_weighted_templated_Fill.cxx)
target_link_libraries(benchmark_double_weighted_templated_Fill EPHist benchmark::benchmark)

add_executable(benchmark_DoubleBinWithError_weighted_Fill DoubleBinWithError_weighted_Fill.cxx)
target_link_libraries(benchmark_DoubleBinWithError_weighted_Fill EPHist benchmark::benchmark)
add_executable(benchmark_DoubleBinWithError_weighted_Fill_tuple DoubleBinWithError_weighted_Fill_tuple.cxx)
target_link_libraries(benchmark_DoubleBinWithError_weighted_Fill_tuple EPHist benchmark::benchmark)
add_executable(benchmark_DoubleBinWithError_weighted_templated_Fill DoubleBinWithError_weighted_templated_Fill.cxx)
target_link_libraries(benchmark_DoubleBinWithError_weighted_templated_Fill EPHist benchmark::benchmark)
