cmake_minimum_required(VERSION 3.16.3)

# name of the project
project(logger)

# set c++ standard to c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ensure that yaml-cpp is installed and include it's directories
add_subdirectory(external/yaml-cpp) # This is one way of referencing another CMake project
   
# compile and link yaml-cpp
add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(
    ${PROJECT_NAME}  
    PRIVATE
    yaml-cpp  # This is the "target" exposed by 
              # yaml-cpp's CMakeLists.txt file
)

# copy the config file to the build directory
configure_file("logger.conf.yaml" "logger.conf" COPYONLY)
