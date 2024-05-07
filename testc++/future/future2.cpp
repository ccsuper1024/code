//
// Created by chenchao on 24-5-7.
//
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int fib(int n)
{
    if (n < 3)
        return 1;
    else
        return fib(n-1) + fib(n - 2);   //递归
}

int main() {
    const auto start = std::chrono::system_clock::now();
    //立即执行std::async
    std::future<int> f1 = std::async(std::launch::async, []() { return fib(40); });
    std::future<int> f2 = std::async(std::launch::async, []() { return fib(43); });

    std::cout << "waiting... " << std::flush;

    f1.wait();
    f2.wait();

    const auto diff = std::chrono::system_clock::now() - start;
    std::cout << std::chrono::duration<double>(diff).count() << " seconds\n";

    std::cout << "f1: " << f1.get() << std::endl;
    std::cout << "f2: " << f2.get() << std::endl;
}