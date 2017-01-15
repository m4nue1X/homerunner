# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-node)

cmake_minimum_required(VERSION 2.8.3)

include("../global.cmake")

add_executable(hrnode
  src/main.cpp
)

target_include_directories(hrnode
  PRIVATE ${LIBHR_INCLUDE_DIRS}
)

target_link_libraries(hrnode
  ${LIBHR_LIBRARIES}
)

