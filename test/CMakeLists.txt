# Unset the module path potentially set by ROOT, we want CMake's version of
# FindGTest.cmake.
unset(CMAKE_MODULE_PATH)

find_package(GTest)
if(NOT GTEST_FOUND)
  message(WARNING "GTest not found, disabling tests")
  return()
endif()

add_executable(test_atomic atomic.cxx)
target_link_libraries(test_atomic EPHist GTest::Main)
add_test(NAME atomic COMMAND test_atomic)

add_executable(test_axes axes.cxx)
target_link_libraries(test_axes EPHist GTest::Main)
add_test(NAME axes COMMAND test_axes)

add_executable(test_basic basic.cxx)
target_link_libraries(test_basic EPHist GTest::Main)
add_test(NAME basic COMMAND test_basic)

add_executable(test_categorical categorical.cxx)
target_link_libraries(test_categorical EPHist GTest::Main)
add_test(NAME categorical COMMAND test_categorical)

add_executable(test_index index.cxx)
target_link_libraries(test_index EPHist GTest::Main)
add_test(NAME index COMMAND test_index)

add_executable(test_parallel parallel.cxx)
target_link_libraries(test_parallel EPHist GTest::Main)
add_test(NAME parallel COMMAND test_parallel)

add_executable(test_profile profile.cxx)
target_link_libraries(test_profile EPHist GTest::Main)
add_test(NAME profile COMMAND test_profile)

add_executable(test_regular regular.cxx)
target_link_libraries(test_regular EPHist GTest::Main)
add_test(NAME regular COMMAND test_regular)

add_executable(test_slicing slicing.cxx)
target_link_libraries(test_slicing EPHist GTest::Main)
add_test(NAME slicing COMMAND test_slicing)

add_executable(test_variable variable.cxx)
target_link_libraries(test_variable EPHist GTest::Main)
add_test(NAME variable COMMAND test_variable)

add_executable(test_weighted weighted.cxx)
target_link_libraries(test_weighted EPHist GTest::Main)
add_test(NAME weighted COMMAND test_weighted)

if(BUILD_UTIL)
  add_executable(test_ExportData ExportData.cxx)
  target_link_libraries(test_ExportData EPHist EPHistUtil GTest::Main)
  add_test(NAME ExportData COMMAND test_ExportData)
endif()

if(BUILD_UTIL_ROOT)
  add_executable(test_ConvertToROOT ConvertToROOT.cxx)
  target_link_libraries(test_ConvertToROOT EPHist EPHistUtilROOT ROOT::Hist GTest::Main)
  add_test(NAME ConvertToROOT COMMAND test_ConvertToROOT)

  add_executable(test_RDataFrameHelper RDataFrameHelper.cxx)
  target_link_libraries(test_RDataFrameHelper EPHist EPHistUtilROOT ROOT::ROOTDataFrame GTest::Main)
  add_test(NAME RDataFrameHelpers COMMAND test_RDataFrameHelper)
endif()
