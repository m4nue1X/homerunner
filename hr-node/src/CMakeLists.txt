# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-node)

cmake_minimum_required(VERSION 2.8.3)

include("../../global.cmake")

add_executable(hrnode
  main.cpp
)

target_link_libraries(hrnode
  dl
  pthread
  homerunner
)
