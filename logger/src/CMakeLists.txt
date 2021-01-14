cmake_minimum_required(VERSION 3.16.3)

# name of the project
project(logger)

# set c++ standard to c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ensure that yaml-cpp is installed and include it's directories
add_subdirectory(external/yaml-cpp) # This is one way of referencing another CMake project


# add local repository libraries
add_library(
    threadable_msg_queue
    ThreadableMsgQueue.hpp
    ThreadableMsgQueue.cpp
)
add_library(
    message
    Message.hpp
    Message.cpp
)

   
# compile
add_executable(${PROJECT_NAME} main.cpp)

# link yaml-cpp
target_link_libraries(
    ${PROJECT_NAME}  
    PRIVATE
    yaml-cpp  # This is the "target" exposed by 
              # yaml-cpp's CMakeLists.txt file
)

target_link_libraries(${PROJECT_NAME} PRIVATE threadable_msg_queue)
target_link_libraries(${PROJECT_NAME} PRIVATE message)

# copy the config file to the build directory
configure_file("logger.conf.yaml" "logger.conf" COPYONLY)
