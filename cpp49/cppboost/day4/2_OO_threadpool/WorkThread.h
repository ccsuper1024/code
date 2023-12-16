/*  @file @brief @author chenchao @brief */
#ifndef  _WORKTHREAD_HPP
#define  _WORKTHREAD_HPP
#include "ThreadPool.h"

class WorkThread
:public Thread      //公有继承,所以workThread也不可复制
{
public:
/* @file @brief @pram*/
    WorkThread(ThreadPool& pool)
    :_pool(pool) {

    }   
    ~WorkThread() {

    }
    //取执行线程池中doTask()
    void run() override {
        //doTask是线程池中的私有成员函数，所以需要将
        //workThread作为ThreadPool的友元
        _pool.doTask();
    }
private:
    ThreadPool& _pool;
};
#endif      //_WORKTHREAD_HPP

