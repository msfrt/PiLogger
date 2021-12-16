cmake_minimum_required(VERSION 3.16.3)

# name of the project
project(logger)

# set c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ensure that yaml-cpp is installed and include its directories
add_subdirectory(external/yaml-cpp) # This is one way of referencing another CMake project

# add dbccpp for dbc decoding (be sure that its dependencies are installed!)
# add_subdirectory(external/dbcppp)


# add local repository libraries
add_library(
    threadable_queue
    ThreadableQueue.hpp
    ThreadableQueue.cpp
)
add_library(
    message
    Message.hpp
    Message.cpp
)
add_library(
    monitor
    monitor.hpp
    monitor.cpp
)
add_library(
    consumer
    consumer.hpp
    consumer.cpp
)
add_library(
    interfaces
    interfaces.hpp
    interfaces.cpp
)




   
# compile
add_executable(${PROJECT_NAME}  main.cpp)


# link yaml-cpp
target_link_libraries(
    ${PROJECT_NAME}  
    PRIVATE
    yaml-cpp  # This is the "target" exposed by 
              # yaml-cpp's CMakeLists.txt file
)

# link pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

target_link_libraries(${PROJECT_NAME} PRIVATE threadable_queue)
target_link_libraries(${PROJECT_NAME} PRIVATE message)
target_link_libraries(${PROJECT_NAME} PRIVATE monitor)
target_link_libraries(${PROJECT_NAME} PRIVATE consumer)
target_link_libraries(${PROJECT_NAME} PRIVATE interfaces)
target_link_libraries(${PROJECT_NAME} PRIVATE dbcppp)


# copy the config file to the build directory
configure_file("logger.conf.yaml" "logger.conf" COPYONLY)
