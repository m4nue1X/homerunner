# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-connector-bluez)

cmake_minimum_required(VERSION 2.8.3)

include("../global.cmake")

add_library(hrconn_bluez SHARED
  src/BluezBTLEConnector.cpp
)

target_include_directories(hrconn_bluez
  PRIVATE ${LIBHR_INCLUDE_DIRS}
)

target_link_libraries(hrconn_bluez
  ${LIBHR_LIBRARIES}
  bluetooth
)
