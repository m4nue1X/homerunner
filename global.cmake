
cmake_minimum_required(VERSION 2.8.3)

#set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-g -fvisibility=hidden ${CMAKE_C_FLAGS}")

## HomeRunner
set(HR_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${HR_ROOT}/installs/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${HR_ROOT}/installs/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HR_ROOT}/installs/bin)

## VNL / VNI
set(VNL_DIR "${HR_ROOT}/hr-lib/lib/virtual-network-layers-cpp")
list(APPEND VNL_INCLUDE_DIRS "${VNL_DIR}/include")
list(APPEND VNL_LIBRARY_DIRS "${HR_ROOT}/installs/lib")
list(APPEND VNL_LIBRARIES "vnl")

if(DEFINED ENV{VNICPPCODEGEN} AND EXISTS "$ENV{VNICPPCODEGEN}")
  set(VNICPPCODEGEN "$ENV{VNICPPCODEGEN}")
else(DEFINED ENV{VNICPPCODEGEN} AND EXISTS "$ENV{VNICPPCODEGEN}")
  set(VNICPPCODEGEN "vnicppcodegen")
endif(DEFINED ENV{VNICPPCODEGEN} AND EXISTS "$ENV{VNICPPCODEGEN}")

## libhr
set(LIBHR_DIR "${HR_ROOT}/hr-lib")
list(APPEND LIBHR_INCLUDE_DIRS "${LIBHR_DIR}/include" "${LIBHR_DIR}/generated/include")
list(APPEND LIBHR_LIBRARY_DIRS "${HR_ROOT}/installs/lib")
list(APPEND LIBHR_LIBRARIES "homerunner")

## Build
include_directories(
    ${VNL_INCLUDE_DIRS}
    ${LIBHR_INCLUDE_DIRS}
)

link_directories(
    ${VNL_LIBRARY_DIRS}
    ${LIBHR_LIBRARY_DIRS}
)
