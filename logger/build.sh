#!/bin/bash

# make sure that g++ compiler is install
sudo apt-get install g++ -y

# install cmake
sudo apt-get install cmake -y

# build and install dbcppp
mkdir src/external/dbcppp/build
cmake -DCMAKE_BUILD_TYPE=Release -Dbuild_tests=OFF -Dbuild_examples=OFF -S src/external/dbcppp -B src/external/dbcppp/build
make -j src/external/dbcppp/build
sudo make install src/external/dbcppp/build
sudo ldconfig










