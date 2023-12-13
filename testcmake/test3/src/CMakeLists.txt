#cmake_minimum_required(VERSION 3.16)
#project(hello CXX) #添加project可以指定顶级目录，使用add_subdirectory()时会要求
set(SOURCE hello.cc)
message(STATUS "This is BINARY dir" ${HELLO_BINARY_DIR})
message(STATUS "This is SOURCE dir" ${HELLO_SOURCE_DIR})
add_executable(hello ${SOURCE})
#第一个单词为关键字，后续两个参数是指定的文件路径，第四个参数指定这些文件的安装路径
#为第五个参数。第五个参数是个相对路径，默认是/usr/local ，可以通过cmake --install-prefix=*来修改
install(FILES ../COPYRIGHT ../README DESTINATION file)
install(PROGRAMS ../runhello.sh DESTINATION bin)
#DIRECTORY 后的是要安装的路径。如果是只有目录，则把整个目录安装到指定路径
#如果是目录名+/则只会安装目录下的文件到指定路径
install(DIRECTORY ../doc/ DESTINATION doc/)

