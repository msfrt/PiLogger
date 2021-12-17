# logger

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [logger](#logger)
- [Automatic Build](#automatic-build)
- [Manual Build](#manual-build)
  - [Dependencies](#dependencies)
    - [Manually installing dbcppp](#manually-installing-dbcppp)
  - [CMake](#cmake)
  - [Build binaries](#build-binaries)
- [Configuration](#configuration)
- [Run](#run)
  - [Command line options](#command-line-options)
    - [`-f`](#-f)

<!-- /code_chunk_output -->

# Automatic Build

To use the logger, you must build it first. To make things nice and easy, I've created a `./build` script that will download, build, and install the dependencies for the logger, as well as build the logger application once dependencies are met. To run this, simply run `sudo ./build` from within the `logger` directory.


# Manual Build

Manual build is accomplished using [CMake](https://cmake.org). If CMake is not installed, you must install it, either by downloading the source and building, or simply running `sudo apt-get install cmake` (easiest). Downloading via the package manager may not install the latest version, so I reccomend building by source. Instructions for doing so can be found [here](https://cmake.org/install/).

## Dependencies

* [yaml-cpp](https://github.com/jbeder/yaml-cpp) is used for YAML file parsing, but is included as a submodule in this repository to make for easy building. If the yaml-cpp files do not appear in the `logger/src/external/yaml-cpp` directory after cloning, cd into that directory and run `git submodule init`, followed by `git submodule update`.
* [influxdb-cpp-2](https://github.com/TheYonkk/influxdb-cpp-2) is required to write to the database. If the influxdb-cpp files do not appear in the `logger/src/external/influxdb-cpp-2` directory after cloning, cd into that directory and run `git submodule init`, followed by `git submodule update`.
* [dbcppp](https://github.com/xR3b0rn/dbcppp) is used for CAN frame decoding, but is included as a submodule in this repository to make for easy building. If the dbcppp files do not appear in the `logger/src/external/dbcppp` directory after cloning, cd into that directory and run `git submodule init`, followed by `git submodule update`. dbccpp has additional subrepos that need to be cloned as well. You can find those in the `third-party` folder in that repo.

### Manually installing dbcppp
dbcppp is built and installed via the `./build` script located in this directory, however, if it fails and you need to install it directly, here are the steps. Follow [these steps](https://github.com/xR3b0rn/dbcppp#build--install) in the dbcppp submodule folder:
  1. `mkdir build`
  2. `cd build`
  3. `cmake -DCMAKE_BUILD_TYPE=Release -Dbuild_tests=OFF -Dbuild_examples=OFF ..`
  4. `make -j`
  5. `sudo make install`
  6. `sudo ldconfig` on unix systems only

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
