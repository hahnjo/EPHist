# Require CMake 3.12 to support the <policy_max> version.
cmake_minimum_required(VERSION 3.12...3.28)

project(EPHist VERSION 0.0.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(EPHist INTERFACE)
target_include_directories(EPHist INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(EPHist INTERFACE cxx_std_17)

install(TARGETS EPHist EXPORT ${PROJECT_NAME}Targets)
# Install header files manually: PUBLIC_HEADER has the disadvantage that CMake
# flattens the directory structure on install, and FILE_SETs are only available
# with CMake v3.23.
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/Atomic.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/DoubleBinWithError.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/EPHist.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/RegularAxis.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/VariableBinAxis.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/Weight.hxx
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/EPHist
)

option(BUILD_UTIL "Build the EPHistUtil library." ON)
if(BUILD_UTIL)
  add_library(EPHistUtil SHARED
    src/ExportData.cxx
  )
  target_include_directories(EPHistUtil INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_link_libraries(EPHistUtil PUBLIC EPHist)

  install(TARGETS EPHistUtil EXPORT ${PROJECT_NAME}Targets)
  install(FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/Util/ExportData.hxx
    DESTINATION include/EPHist/Util
  )
endif()

find_package(ROOT QUIET COMPONENTS Hist ROOTDataFrame)
option(BUILD_UTIL_ROOT "Build the EPHistUtilROOT library." ${ROOT_FOUND})
if(BUILD_UTIL_ROOT)
  if(NOT ROOT_FOUND)
    # Call find_package again with the REQUIRED keyword to print an error.
    find_package(ROOT REQUIRED COMPONENTS Hist ROOTDataFrame)
    message(FATAL_ERROR "EPHistUtilROOT enabled, but ROOT not found")
  endif()

  add_library(EPHistUtilROOT SHARED
    src/ConvertToROOT.cxx
  )
  target_include_directories(EPHistUtilROOT INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_link_libraries(EPHistUtilROOT PUBLIC EPHist ROOT::Hist)

  install(TARGETS EPHistUtilROOT EXPORT ${PROJECT_NAME}Targets)
  install(FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/Util/ConvertToROOT.hxx
      ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/Util/RDataFrameHelper.hxx
    DESTINATION include/EPHist/Util
  )
endif()

export(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
)
install(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EPHist)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/EPHistConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EPHist
  NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/EPHistConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EPHist
)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

option(BUILD_BENCHMARKS "Build the benchmarks." ON)
if(BUILD_BENCHMARKS)
  add_subdirectory(benchmark)
endif()
