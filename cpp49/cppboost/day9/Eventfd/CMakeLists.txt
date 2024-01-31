cmake_minimum_required(VERSION 3.20)

project(testThread)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGE "${CMAKE_CXX_FLAGE} -g -Wall -pthread")
add_executable(testThread testthread.cc Thread.cc)

project(testEventfd)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGE "${CMAKE_CXX_FLAGE} -g -Wall -pthread")
add_executable(testEventfd TestEventFd.cc Thread.cc EventFd.cc)