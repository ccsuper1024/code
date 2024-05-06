/*
std::call_once 等同于POSIX中的pthread_once
c++:
std::once_flag& flag;       //std::once_flag类的构造函数是constexpr的，是常量构造
template< class Callable, class... Args >
void call_once( std::once_flag& flag, Callable&& f, Args&&... args );
Linux:
#include<pthread.h>
pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t* once_control, void(*init_routine)(void));
*/
#include <iostream>
#include <mutex>
#include <thread>


std::once_flag flag1,flag2;
void simple_do_once() {
    std::call_once(flag1, [](){ std::cout << "Simple example: called once\n";   });
}

void may_throw_function(bool do_throw) {
    if(do_throw) {
        std::cout << "Throw: call_once will retry \n";  //  this may appear more than once
        throw std::exception();
    }
    std::cout << "Did not throw, call_once will not attempt again\n";   //guarantee once
}

void do_once(bool do_throw) {
    try{
        std::call_once(flag2, may_throw_function, do_throw);
    }
    catch(...) {
    }
}

int main()
{
    std::thread st1(simple_do_once);
    std::thread st2(simple_do_once);
    std::thread st3(simple_do_once);
    std::thread st4(simple_do_once);
    st1.join();
    st2.join();
    st3.join();
    st4.join();
    std::thread t1(do_once, true);
    std::thread t2(do_once, true);
    std::thread t3(do_once, false);
    std::thread t4(do_once, true);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}