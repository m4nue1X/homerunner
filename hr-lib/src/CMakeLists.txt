# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-lib)

cmake_minimum_required(VERSION 2.8.3)

include("../../global.cmake")

# create list of generated source files
file(GLOB_RECURSE LIBHR_GENERATED_SOURCES "../generated/src/*.cxx")

# set visibility default for library
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=default")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=default")

add_library(homerunner SHARED
  ${LIBHR_GENERATED_SOURCES}
)

target_link_libraries(homerunner
  ${VNL_LIBRARIES}
)
