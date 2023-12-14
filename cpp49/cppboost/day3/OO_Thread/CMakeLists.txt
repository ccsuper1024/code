cmake_minimum_required(VERSION 3.20)

project(oo_thread)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -lpthread")

add_executable(oo_thread Thread.cc TestThread.cc)