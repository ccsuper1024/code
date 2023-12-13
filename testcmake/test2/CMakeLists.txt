cmake_minimum_required(VERSION 3.16)
project(hello CXX) #添加project可以指定顶级目录，使用add_subdirectory()时会要求
set(SOURCE ./src/hello.cc)
message(STATUS "This is BINARY dir" ${HELLO_BINARY_DIR})
message(STATUS "This is SOURCE dir" ${HELLO_SOURCE_DIR})
add_executable(hello ${SOURCE})
