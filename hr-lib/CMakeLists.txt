# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-lib)

cmake_minimum_required(VERSION 2.8.3)

include("../global.cmake")

# create list of generated source files
file(GLOB_RECURSE LIBHR_GENERATED_SOURCES "./generated/src/*.cxx")
file(GLOB_RECURSE VNL_SOURCES "./lib/virtual-network-layers-cpp/src/*.cpp")

add_library(homerunner SHARED
  ${LIBHR_GENERATED_SOURCES}
  ${VNL_SOURCES}
)

target_include_directories(homerunner
  PUBLIC ${LIBHR_INCLUDE_DIRS}
)

# set visibility default for library
target_compile_options(homerunner PUBLIC "-fvisibility=default")
