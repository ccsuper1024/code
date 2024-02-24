cmake_minimum_required(VERSION 3.10)
project(my_project)

#Abseil requires c++14
set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include/absl)

#Process Abseil's Cmake build system
add_executable(hello_world test.cpp)

target_link_libraries(hello_world libabsl.so)
