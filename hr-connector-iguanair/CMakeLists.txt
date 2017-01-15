# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-connector-iguanair)

cmake_minimum_required(VERSION 2.8.3)

include("../global.cmake")

add_library(hrconn_igir SHARED
  src/IguanaIRConnector.cpp
)

target_include_directories(hrconn_igir
  PRIVATE ${LIBHR_INCLUDE_DIRS}
)

target_link_libraries(hrconn_igir
  ${LIBHR_LIBRARIES}
  iguanaIR
)
