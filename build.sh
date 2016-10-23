#!/bin/sh

cd "`dirname \"$0\"`"
DIR="`pwd`"

export VNICPPCODEGEN="${DIR}/../virtual-network-interface/installs/bin/vnicppcodegen"

mkdir build 2>/dev/null
cd build
cmake ../

make
