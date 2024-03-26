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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/EPHist.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/RegularAxis.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/VariableBinAxis.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EPHist/Weight.hxx
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/EPHist
)

export(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
)
install(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EPHist)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/EPHistConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EPHist
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
