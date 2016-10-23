#!/bin/bash

if [ -d "${1}" ]; then
  echo "Usage: createeclipseproject.sh <project-root-dir>"
fi

cd "${1}"

cmake -G "Eclipse CDT4 - Unix Makefiles"

rm -Rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile src/CMakeFiles src/cmake_install.cmake src/Makefile
