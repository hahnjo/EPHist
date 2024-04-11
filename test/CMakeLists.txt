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
