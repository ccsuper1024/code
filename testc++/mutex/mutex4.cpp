//递归锁提供独占的，递归的锁。
//递归锁可以被递归加锁的次数的未指定的，当如果这个值到达了，调用lock()就会抛出std::system_error，调用try_lock()将会返回false
//当锁被销毁时，仍被某些锁拥有或者线程带着锁陷入阻塞或线程带着锁被销毁都是未定义的

//递归锁的一个用例是，在一个成员函数互相调用的类中保护共享数据
#include <iostream>
#include <mutex>
#include <thread>

class Object
{
private:
    std::recursive_mutex recur_mutex;
    std::string shared;
public:
    void fun1() {
        std::lock_guard<std::recursive_mutex> lk(recur_mutex);  //基于范围的锁管理
        shared = "fun1";
        std::cout << "in fun1, shared variable is now " << shared << "\n";
    }

    void fun2(){
        std::lock_guard<std::recursive_mutex> lk(recur_mutex);
        shared = "fun2";
        std::cout << "in fun2, shared variable is now " << shared << "\n";
        fun1(); //recursive lock becomes useful here
        std::cout << "back in fun2, shared variable is " << shared << "\n";
    }
};

int main(void) {
    Object ob;
    std::thread t1(&Object::fun1,&ob);
    std::thread t2(&Object::fun2,&ob);
    t1.join();
    t2.join();
}