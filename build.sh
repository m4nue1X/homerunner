#!/bin/sh

cd "`dirname \"$0\"`"
DIR="`pwd`"

export VNL_ROOT="${DIR}/../virtual-network-layers-cpp"
export VNICPPCODEGEN="${DIR}/../virtual-network-interface/installs/bin/vnicppcodegen"

mkdir build 2>/dev/null
cd build
cmake ../

make
