#include <iostream>
#include <mutex>
#include <string_view>
#include <syncstream>
#include <thread>

/*
volatile 因为一般来说，编译器会把变量优化到寄存器中，而不是存入内存(栈上)，而volatile用于告诉编译器不要进行这样的优化，
一般volaitile用于多线程竞争的共享变量，中断程序等
*/
volatile int g_i = 0;
std::mutex g_i_mutex;   //protect g_i

void safe_increment(int iterations) {
    const std::lock_guard<std::mutex> lock(g_i_mutex);
    while(iterations-- > 0) {   //后缀递减
        g_i = g_i + 1;
    }
    std::cout << "thread #" << std::this_thread::get_id() << ", g_i: " << g_i << '\n';
    //g_i_mutex is automatically released when lock goes out of scope
}

void unsafe_increment(int iterations) {
    while (iterations-- > 0) {
        g_i += 1;
    }
    std::osyncstream(std::cout) << "thread #" << std::this_thread::get_id() << ", g_i: " << '\n';
}

int main(void) {
    //std::string_view 是个视图类，它操作的是指针，一般赋值为一个字符串常量
    auto test = [](std::string_view fun_name, auto fun) {
        g_i = 0;
        std::cout << fun_name << ": \nbefore, g_i: " << g_i << '\n';
        {
            std::jthread t1(fun, 1'000'000);
            std::jthread t2(fun, 1'000'000);
        }
        std::cout << "after, g_i: " << g_i << "\n\n";
    };
    test("safe_increment", safe_increment);
    test("unsafe_increment", unsafe_increment);
}