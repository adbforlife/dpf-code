cmake_minimum_required(VERSION 3.10)

# set the project name
project(Dpf VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenSSL REQUIRED)

add_library(Util utils.cpp)

# add the executable
add_executable(Dpf dpf-tree.cpp)

target_link_libraries(Dpf PUBLIC OpenSSL::SSL)
target_link_libraries(Dpf PUBLIC Util)