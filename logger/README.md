# logger

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [logger](#logger)
- [Build](#build)
  - [Dependencies](#dependencies)
    - [Installing yaml-cpp](#installing-yaml-cpp)
    - [Installing dbcppp](#installing-dbcppp)
  - [CMake](#cmake)
  - [Build binaries](#build-binaries)
- [Configuration](#configuration)
- [Run](#run)
  - [Command line options](#command-line-options)
    - [`-f`](#-f)

<!-- /code_chunk_output -->


# Build

To use the logger, you must build it first. This is accomplished using [CMake](https://cmake.org). If CMake is not installed, you must install it, either by downloading the source and building, or simply running `sudo apt-get install cmake` (easiest). Downloading via the package manager may not install the latest version, so I reccomend building by source. Instructions for doing so can be found [here](https://cmake.org/install/).

## Dependencies

* [yaml-cpp](https://github.com/jbeder/yaml-cpp) is used for YAML file parsing, but is included as a submodule in this repository to make for easy building. If the yaml-cpp files do not appear in the `logger/src/external/yaml-cpp` directory after cloning, cd into that directory and run `git submodule init`, followed by `git submodule update`.
* [influxdb-cpp-2](https://github.com/TheYonkk/influxdb-cpp-2) is required to write to the database. If the influxdb-cpp files do not appear in the `logger/src/external/influxdb-cpp-2` directory after cloning, cd into that directory and run `git submodule init`, followed by `git submodule update`.
* [dbcppp](https://github.com/xR3b0rn/dbcppp) is used for CAN frame decoding, but is included as a submodule in this repository to make for easy building. If the dbcppp files do not appear in the `logger/src/external/dbcppp` directory after cloning, cd into that directory and run `git submodule init`, followed by `git submodule update`. dbccpp has two dependencies that you need to manually install:
  * Boost 1.72.0 or greater (instructions from [here](https://stackoverflow.com/a/24086375))
    1. `wget -O boost_1_72_0.tar.gz https://sourceforge.net/projects/boost/files/boost/1.72.0/boost_1_72_0.tar.gz/download`
    1. `tar xzvf boost_1_72_0.tar.gz`
    1. `cd boost_1_72_0/`
    1. `sudo apt-get update`
    1. `sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev libboost-all-dev`
    1. `./bootstrap.sh --prefix=/usr/`
    1. `./b2`
    1. `sudo ./b2 install`
  * LibXml2
    1. `sudo apt-get install libxml2-dev`


### Installing yaml-cpp
There is nothing that you need to do to install yaml-cpp after the submodule is cloned. The logger CMake command and the resultant Makefiles handle all compilation and linking.

### Installing dbcppp
Installing dbcppp is less straightforward, but still easy. Once you've installed Boost and LibXml2 as [described above](#dependencies), follow [these steps](https://github.com/xR3b0rn/dbcppp#build--install) in the dbcppp submodule folder:
  1. `mkdir build`
  1. `cd build`
  1. `cmake -DCMAKE_BUILD_TYPE=Release -Dbuild_tests=OFF -Dbuild_examples=OFF ..`
  1. `make -j`
  1. `sudo make install`
  1. `sudo ldconfig` on unix systems only

## CMake
CMake creates the Makefiles necessary for UNIX building. To run CMake:
1. `cd logger/src`
2. `mkdir build`
3. `cd build`
4. `cmake ..`

The Makefiles should now be present if all prerequisits were satisfied.

## Build binaries

After running CMake, you're all set. Simply run the command `make` from within the `build` directory.

# Configuration

TDB

# Run

To run the logger, simply run `./logger` after building

## Command line options

### `-f`
The default configuration file is `logger.conf`, located in the build directory after building. If you'd like to use a different configuration file, use the `-f` flag. For example: `./logger -f /path/to/my.config`
