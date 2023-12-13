include_directories(/usr/local/include/sayhello)
add_executable(test ./testsayhello.cc)
#如果是自定的库路径，不再系统指定的库路径之下，则要用link_directories()
target_link_libraries(test libsayhello.so)
