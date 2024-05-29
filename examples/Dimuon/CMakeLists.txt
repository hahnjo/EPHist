# Require CMake 3.12 to support the <policy_max> version.
cmake_minimum_required(VERSION 3.12...3.28)

project(Dimuon LANGUAGES CXX)

find_package(ROOT REQUIRED COMPONENTS Hist ROOTDataFrame)
find_package(EPHist REQUIRED COMPONENTS Util UtilROOT)

add_executable(analysis_add analysis_add.cxx)
target_link_libraries(analysis_add EPHist::EPHist EPHist::EPHistUtil EPHist::EPHistUtilROOT ROOT::Hist ROOT::ROOTDataFrame)

add_executable(analysis_atomic analysis_atomic.cxx)
target_link_libraries(analysis_atomic EPHist::EPHist EPHist::EPHistUtil EPHist::EPHistUtilROOT ROOT::Hist ROOT::ROOTDataFrame)
