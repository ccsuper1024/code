cmake_minimum_required(VERSION 3.22)
#创建工程
project(main)
#c++版本为17
set(CMAKE_CXX_FLAGS "-std=17")

#寻找OpenCV 4.1的安装路径
find_package(OpenCV 4.6 REQUIRED)
#加载OpenCV4.1的头文件
include_directories(${OpenCV_INCLUDE_DIRS})
#生成可执行文件
add_executable(main 1-1.cpp)
#链接lib文件到可执行文件
target_link_directories(main ${OpenCV_LIBS})