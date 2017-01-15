
cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -fvisibility=hidden -pthread -Wfatal-errors -fPIC -z defs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fvisibility=hidden -pthread -Wfatal-errors -fPIC -z defs")

## HomeRunner
set(HR_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${HR_ROOT}/installs/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${HR_ROOT}/installs/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HR_ROOT}/installs/bin)

## libhr
set(LIBHR_DIR "${HR_ROOT}/hr-lib")
list(APPEND LIBHR_INCLUDE_DIRS "${LIBHR_DIR}/include" "${LIBHR_DIR}/generated/include" "${LIBHR_DIR}/lib/virtual-network-layers-cpp/include")
list(APPEND LIBHR_LIBRARIES "${HR_ROOT}/installs/lib/libhomerunner.so")

include("${HR_ROOT}/gitversion.cmake")

