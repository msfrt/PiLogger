#!/bin/bash

# make sure that g++ compiler is install
sudo apt-get install g++ -y

# install cmake
sudo apt-get install cmake -y

# build and install dbcppp
mkdir src/external/dbcppp/build
cd src/external/dbcppp/build
cmake -DCMAKE_BUILD_TYPE=Release -Dbuild_tests=OFF -Dbuild_examples=OFF ..
make -j
sudo make install
sudo ldconfig

# build the logger application
mkdir ../../../build   # (PiLogger/logger/src/build)
cd ../../../build
cmake ..
make










