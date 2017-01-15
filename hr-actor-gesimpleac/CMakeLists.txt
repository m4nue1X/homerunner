# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-actor-gesimpleac)

cmake_minimum_required(VERSION 2.8.3)

include("../global.cmake")

add_library(hract_gesimpleac SHARED
  src/GESimpleAirCondition.cpp
)

target_include_directories(hract_gesimpleac
  PRIVATE ${LIBHR_INCLUDE_DIRS}
)

target_link_libraries(hract_gesimpleac
  ${LIBHR_LIBRARIES}
)

