cmake_minimum_required(VERSION 3.20)
project(bo_thread)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")
add_executable(bo_thread Thread.cc TestThread.cc)
target_link_libraries(bo_thread pthread)