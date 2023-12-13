set(CMAKE_CXX_STANDARD 11)
set(LIBSAYHELLO_SRC sayhello.cc)
add_library(sayhello_static STATIC ${LIBSAYHELLO_SRC})

#对sayhello_static 的重命名为sayhello
set_target_properties(sayhello_static PROPERTIES OUTPUT_NAME sayhello)
#在cmake构建一个新的target时，会尝试清理掉其他使用这个名字的库，因为，在构建
#libsayhello.so时，就会清理掉 libsayhello.a.所以只是简单的调用两次add_library
#来同时构建动态库和静态库时才只会生成一个动态库
set_target_properties(sayhello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#构建动态库
add_library(sayhello SHARED ${LIBSAYHELLO_SRC})

set_target_properties(sayhello PROPERTIES OUTPUT_NAME "sayhello")
#VERSION 指定动态库的版本号， SOVERSION 指定API版本
set_target_properties(sayhello PROPERTIES VERSION .01 SOVERSION 1)
set_target_properties(sayhello PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#安装
file(GLOB INCLUDE_FILE *.h)

#把文件放到目录下，使用FILES参数
install(FILES ${INCLUDE_FILE} DESTINATION include/sayhello)
#把二进制，静态库，动态库安装，用TARGETS参数
#ARCHIVE 特指静态库 LIBRARY 特指动态库 RUNTIME特指可执行目标二进制
install(TARGETS sayhello sayhello_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

