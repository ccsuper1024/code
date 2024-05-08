//
// Created by chenchao on 24-5-8.
//
#include "ThreadPool.h"
#include <iostream>
#include <functional>

class MyTask
{
public:
    void print()
    {
        std::cout << "I am running" <<std::endl;
    }
};

int main()
{
    MyTask mytask;
    ThreadPool threadpool(2,5);
    threadpool.start();
    for(int i{}; i < 20; ++i) {
        threadpool.addTask(std::move(std::bind(&MyTask::print,&mytask)));
        std::cout << "count = " << i << std::endl;
    }

    threadpool.stop();
}