cmake_minimum_required(VERSION 3.16)
project(hello) #项目名称一般只有一个
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-g -Wunused)              #全局的编译选项
#target_compile_options(main PUBLIC-g -Werror) #指定目标的编译选项
add_executable(main hello.cc)

