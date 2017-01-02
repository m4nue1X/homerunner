# cmake -G "Eclipse CDT4 - Unix Makefiles"

project(hr-engine-ecmascript)

cmake_minimum_required(VERSION 2.8.3)

include("../global.cmake")

add_library(hreng_ecma SHARED
  src/ECMAScriptEngine.cpp
  lib/duktape-src/duktape.c
)

target_include_directories(hreng_ecma
  PRIVATE ${LIBHR_INCLUDE_DIRS} lib/duktape-src
)

target_link_libraries(hreng_ecma
  ${LIBHR_LIBRARIES}
)
