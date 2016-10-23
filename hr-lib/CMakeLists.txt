# cmake -G "Eclipse CDT4 - Unix Makefiles"

cmake_minimum_required(VERSION 2.8.3)

project(hr-lib)

include(../global.cmake)
## override visibility
set(CMAKE_CXX_FLAGS "-fvisibility=default ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-fvisibility=default ${CMAKE_C_FLAGS}")

## Genertate code
message(STATUS "Running ${VNICPPCODEGEN} ${VNL_INTERFACE_DIRS} interface")
execute_process(
  COMMAND ${VNICPPCODEGEN} ${VNL_INTERFACE_DIRS} interface
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
  RESULT_VARIABLE VNIGENRESULT
)
if("${VNIGENRESULT}" EQUAL "0")
  message(STATUS "  success.")
else("${VNIGENRESULT}" EQUAL "0")
  message(FATAL_ERROR "  failed to generate code!")
endif("${VNIGENRESULT}" EQUAL "0")  

include("${VNL_ROOT}/CMakeLists.txt")
add_subdirectory(src)
