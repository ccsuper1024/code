cmake_minimum_required(VERSION 3.16)
project(hello)
 #第一个参数指定src子目录到工程中编译，
 #第二个参数指定相对的二进制目录，其路径前缀为cmake -B指定
add_subdirectory(src bin)  
