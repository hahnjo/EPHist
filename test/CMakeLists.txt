find_package(GTest)
if(NOT GTEST_FOUND)
  message(WARNING "GTest not found, disabling tests")
  return()
endif()

add_executable(test_basic basic.cxx)
target_link_libraries(test_basic EPHist GTest::Main)
add_test(NAME basic COMMAND test_basic)

add_executable(test_regular regular.cxx)
target_link_libraries(test_regular EPHist GTest::Main)
add_test(NAME regular COMMAND test_regular)