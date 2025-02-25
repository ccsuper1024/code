#include <chrono>   //日期与时间
#include <iostream>
#include <thread>
#include <utility>

void f1(int n)  //值拷贝
{
    for (int i = 0; i < 5 ; ++i) {
        std::cout << "Thread 1 executing\n";
        ++n;    //前缀自增
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //睡眠10ms
    }
}

void f2(int& n) //左值引用
{
    for(int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

class foo
{
public:
    void bar() {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 3 executing\n";
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
public:
    int n = 0;
};

class baz
{
public:
    void operator()(){  //重载函数调用运算符函数
        for(int i = 0; i < 5; ++i) {
            std::cout << "Thread 4 executing\n";
            ++n;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

    }

public:
    int n = 0;
};


int main(void ){
    int n = 0;
    foo f;
    baz b;
    std::thread t1; //t1 is not a thread
    std::thread t2(f1, n+1);    //thread是对象语义，不是值语义。 pass by value
    std::thread t3(f2, std::ref(n)); //pass by reference. 应该会调用std::bind。而std::bind默认拷贝参数而不是引用参数，所以对于引用传递要用std::ref算法转换，对于常量引用则可以用std::cref算法
    std::thread t4(std::move(t3));  //t4 is now running f2().t3 is no longer a thread.std::thread类是不可复制的
    std::thread t5(&foo::bar,&f);   //传递类成员函数是，还要传递类对象或对象的地址信息
    std::thread t6(b); //t6 runs baz::operator() on a copy of object b. 传递一个可调用对象，会被std::thread内部的std::function函数对象获取

    t2.join();
    t4.join();
    t5.join();
    t6.join();

    std::cout << "Final value of n is " << n << '\n';
    std::cout << "Final value of f.n (foo::n) is " << f.n << '\n';
    std::cout << "Final value of b.n (baz::n) is " << b.n << std::endl;
}