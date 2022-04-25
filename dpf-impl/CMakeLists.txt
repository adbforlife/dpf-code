cmake_minimum_required(VERSION 3.10)

# set the project name
project(Dpf VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
find_package(OpenSSL REQUIRED)
find_package(GTest REQUIRED)

add_library(Util utils.cpp)
add_library(DpfTree dpf-tree.cpp)

# add the executable
add_executable(test-dpf tests.cpp)
target_link_libraries(test-dpf Util DpfTree)
target_link_libraries(test-dpf OpenSSL::SSL)
target_link_libraries(test-dpf GTest::gtest GTest::gtest_main)

#target_link_libraries(dpf PUBLIC OpenSSL::SSL)
#target_link_libraries(dpf PUBLIC Util)