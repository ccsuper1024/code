/*  @file @brief @author chenchao @brief */
#ifndef  _CONSUMER_HPP
#define  _CONSUMER_HPP
#include "Thread.h"
#include "TaskQueue.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;
class Consumer
:public Thread
{
public:
/* @file @brief @pram*/
    Consumer(TaskQueue& taskque)
    :_taskque(taskque)
    {

    }
    ~Consumer() {}
    void run() override final {
        int cnt = 20;
        while(cnt--) {
            int tmp = _taskque.pop();
            cout << "Consumer consume tmp = " << tmp << endl;
            sleep(1);
        }
    }
private:
    //引用成员
    TaskQueue& _taskque;
};
#endif      //_CONSUMER_HPP

