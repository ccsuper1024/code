//
// Created by chenchao on 24-5-8.
//
#include <iostream>
#include <thread>
#include "TaskQue.h"

void print()
{
    std::cout << " running " << std::endl;
}

int main()
{
    TaskQue taskQue(2);
//    taskQue.wakeup();
    std::thread t1(&TaskQue::push, &taskQue,print);
    std::cout << "push " << std::endl;
    taskQue.pop();
    std::cout << "pop " << std::endl;

    //主线程等待子线程结束
    t1.join();

    return 0;
}