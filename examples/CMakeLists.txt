# Make examples find the current build.
set(EPHist_DIR "${PROJECT_BINARY_DIR}")

if(BUILD_UTIL_ROOT)
  add_subdirectory(Dimuon)
endif()
