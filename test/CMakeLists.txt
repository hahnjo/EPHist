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

add_executable(test_basic basic.cxx)
target_link_libraries(test_basic EPHist GTest::Main)
add_test(NAME basic COMMAND test_basic)

add_executable(test_regular regular.cxx)
target_link_libraries(test_regular EPHist GTest::Main)
add_test(NAME regular COMMAND test_regular)

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
endif()
