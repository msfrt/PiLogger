cmake_minimum_required(VERSION 3.16.3)

# name of the project
project(logger)

# set c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ensure that yaml-cpp is installed and include it's directories
find_package(yaml-cpp)
include_directories(${YAMLCPP_INCLUDE_DIRS})
   
# compile and link yaml-cpp
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(logger ${YAMLCPP_LIBRARIES})

# copy the config file to the build directory
configure_file("logger.conf.yaml" "logger.conf" COPYONLY)
