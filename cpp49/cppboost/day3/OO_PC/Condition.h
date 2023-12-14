/*  @file @brief @author chenchao @brief */
#ifndef  _CONTION_HPP
#define  _CONTION_HPP
#include "MutexLock.h"

class Condition
{
public:
/* @file @brief @pram*/
    Condition(MutexLock& mutex);
    ~Condition();

    //至少唤醒一个线程
    void notify();
    //唤醒所有等待在条件变量中的线程
    void notifyAll();

    //阻塞等待
    void wait();

private:
    //条件变量需要互斥锁的支持
    MutexLock& _mutex;
    //条件变量
    pthread_cond_t _cond;
};
#endif      //_CONTION_HPP

