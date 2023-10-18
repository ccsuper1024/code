cmake_minimum_required(VERSION 3.16)
project(leveldb_Test)
set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)

find_library(LIBS leveldb /usr/local/lib)
find_library(LIBS2 snappy /usr/lib)

add_executable(leveldb_Test test.cpp)

target_link_libraries(leveldb_Test ${LIBS}${LIBS2})
