#!/bin/sh

mkdir build
cd build
qmake -r ../phast.pro
make
cd ..
mkdir deployed
./deploy.sh