/*  @file @brief @author chenchao @brief */
#ifndef  _PRODUCER_HPP
#define  _PRODUCER_HPP
#include "Thread.h"
#include "TaskQueue.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class Producer
:public Thread  //公有继承
{
public:
/* @file @brief @pram*/
    Producer(TaskQueue& taskque)
    :_taskque(taskque)
    {}
    ~Producer() {}
    void run() override final {
        int cnt = 20;
        while (cnt--) {
            _taskque.push(cnt);
            cout << "Producer produce value = " << cnt << endl;
            sleep(1);
        }
    }
private:
    TaskQueue& _taskque;    //和消费者共用同一个任务队列
};
#endif      //_PRODUCER_HPP

