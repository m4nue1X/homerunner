#!/bin/bash

if [ -d "${1}" ]; then
  echo "Usage: createeclipseproject.sh <project-root-dir>"
fi

cd "${1}"

cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=3.8 -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER_ARG1=-std=c++11 ./src/

rm -Rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile src/CMakeFiles src/cmake_install.cmake src/Makefile
