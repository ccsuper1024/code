//查询当前系统支持的并发线程数，如果这个值没有被定义，或者不可计算，则返回0
#include<iostream>
#include<thread>

int main(void) {
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << " Concurrent threads are supported. And the Concurrent threads is "<< n << std::endl;
}