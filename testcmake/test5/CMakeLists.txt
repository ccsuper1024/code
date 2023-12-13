cmake_minimum_required(VERSION 3.12)
project(MyLibraryProject)

#添加源文件
add_library(mylibrary SHARED src/mylibrary.cc)

#设置生成的库文件路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

#生成动态库
#指定别名。把mylibrary库，即libmylibrary.so指定别名为libMyLibrary::mylibrary.so
add_library(MyLibrary::mylibrary ALIAS mylibrary)

#安装动态库到指定目录
install(TARGETS mylibrary LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)


