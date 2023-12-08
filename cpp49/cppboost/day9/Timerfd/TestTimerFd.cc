/* @author chenchao @date @file @brief*/
#include "TimerFd.h"
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

class Task
{
public:
    void process(){
        printf("Task is running \n");
    }    
};

/* @brief @pram @return */
void test(){
    Task mytask;
    //设定起始时间为1，间隔时间为6秒的定时器
    TimerFd tfd(1,6,std::bind(&Task::process,&mytask));

    //自己创建的线程执行handleRead，也就是read操作
    std::thread t(std::bind(&TimerFd::start,&tfd));
    sleep(30);

    tfd.stop();
    t.join();
}
int main()
{
    test();
    return 0;
}

