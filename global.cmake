
cmake_minimum_required(VERSION 2.8.3)

#set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-fvisibility=hidden -std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-fvisibility=hidden ${CMAKE_C_FLAGS}")

## HomeRunner
set(HR_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${HR_ROOT}/installs/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${HR_ROOT}/installs/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HR_ROOT}/installs/bin)

## VNL / VNI
if(DEFINED ENV{VNL_ROOT} AND IS_DIRECTORY "$ENV{VNL_ROOT}")
  set(VNL_ROOT "$ENV{VNL_ROOT}")
else(DEFINED ENV{VNL_ROOT} AND IS_DIRECTORY "$ENV{VNL_ROOT}")
  message(FATAL_ERROR "VNL_ROOT environment variable not defined or not pointing to a directory")
endif(DEFINED ENV{VNL_ROOT} AND IS_DIRECTORY "$ENV{VNL_ROOT}")

list(APPEND VNL_INTERFACE_DIRS "${VNL_ROOT}/interface")
list(APPEND VNL_INCLUDE_DIRS "${VNL_ROOT}/include" "${VNL_ROOT}/generated/include")
list(APPEND VNL_LIB_DIRS "${HR_ROOT}/installs/lib")
list(APPEND VNL_LIBS "vnl")

if(DEFINED ENV{VNICPPCODEGEN} AND EXISTS "$ENV{VNICPPCODEGEN}")
  set(VNICPPCODEGEN "$ENV{VNICPPCODEGEN}")
else(DEFINED ENV{VNICPPCODEGEN} AND EXISTS "$ENV{VNICPPCODEGEN}")
  set(VNICPPCODEGEN "vnicppcodegen")
endif(DEFINED ENV{VNICPPCODEGEN} AND EXISTS "$ENV{VNICPPCODEGEN}")

## libhr
set(LIBHR_DIR "${HR_ROOT}/hr-lib")
list(APPEND LIBHR_INCLUDE_DIRS "${LIBHR_DIR}/include" "${LIBHR_DIR}/generated/include")
list(APPEND LIBHR_LIB_DIRS "${HR_ROOT}/installs/lib")
list(APPEND LIBHR_LIBS "hr")

## Build
include_directories(
    ${VNL_INCLUDE_DIRS}
    ${LIBHR_INCLUDE_DIRS}
)

link_directories(
    ${VNL_LIB_DIRS}
    ${LIBHR_LIB_DIRS}
)
