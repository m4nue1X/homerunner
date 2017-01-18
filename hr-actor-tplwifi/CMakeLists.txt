# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-actor-tplwifi)

cmake_minimum_required(VERSION 2.8.3)

include("../global.cmake")

add_library(hract_tplwifi SHARED
  src/TPLWifi.cpp
  src/TPLWifiSwitchablePlug.cpp
)

target_include_directories(hract_tplwifi
  PRIVATE ${LIBHR_INCLUDE_DIRS} lib/rapidjson/include
)

target_link_libraries(hract_tplwifi
  ${LIBHR_LIBRARIES}
)

add_executable(jsoncoder
  test/jsoncoder.cpp
)