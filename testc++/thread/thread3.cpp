#include<chrono>
#include<iostream>
#include<thread>

using namespace std::chrono_literals;

void foo(){
    std::this_thread::sleep_for(std::chrono::microseconds(500));    //500ms
}

int main(void){
    std::cout << std::boolalpha;

    std::thread t;
    std::cout << "Before starting, joinable: " << t.joinable() << std::endl;

    t = std::thread(foo);   //移动语义
    std::cout << "after starting, joinable: " << t.joinable() <<  std::endl;

    t.join();   //主线程等待子线程结束
    std::cout << "after joining, joinable: " << t.joinable() <<  std::endl;

    t = std::thread(foo);
    t.detach(); //分离线程，主线程不等待子线程结束
    std::cout << "after detaching, joinable: " << t.joinable() <<  std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1500));       //1500ms
}