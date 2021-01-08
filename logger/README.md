# logger <!-- omit in toc -->

- [Build](#build)
  - [Dependencies](#dependencies)
  - [CMake](#cmake)
  - [Build binaries](#build-binaries)
- [Configuration](#configuration)
- [Run](#run)
  - [Command line options](#command-line-options)
    - [`-f`](#-f)

# Build

To use the logger, you must build it first. This is accomplished using [CMake](https://cmake.org). If CMake is not installed, you must install it, either by downloading the source and building, or simply running `sudo apt-get install cmake` (easiest).

## Dependencies

* [yaml-cpp](https://github.com/jbeder/yaml-cpp) is used for YAML file parsing, but is included as a submodule in this repository to make for easy building. If the yaml-cpp files do not appear in the `logger/src/external/yaml-cpp` directory after cloning, cd into that directory and run `git submodule init`, followed by `git submodule update`.

## CMake
CMake creates the Makefiles files necessary for UNIX building. To run CMake:
1. `cd logger/src`
1. `mkdir build`
1. `cd build`
1. `cmake ..`

The Makefiles should now be present if all prerequisits were satisfied.

## Build binaries

After running CMake, you're all set. Simply run the command `make`.

# Configuration

TDB

# Run

To run the logger, simply run `./logger` after building

## Command line options

### `-f`
The default configuration file is `logger.conf`, located in the build directory after building. If you'd like to use a different configuration file, use the `-f` flag. For example: `./logger -f /path/to/my.config`
