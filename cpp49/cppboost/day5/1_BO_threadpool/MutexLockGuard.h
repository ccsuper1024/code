/*  @file @brief @author chenchao @brief */
#ifndef  _MUTEXLOCKGUARD_HPP
#define  _MUTEXLOCKGUARD_HPP
#include "MutexLock.h"
#include "NoCopyable.h"
class MutexLockGuard
:public NoCopyable
{
public:
/* @file @brief @pram*/
    //RAII resource acquistion is initialization
    //资源获取即初始化
    //利用对象的生命周期来管理资源
    MutexLockGuard(MutexLock& mutex)
    :_mutex(mutex)
    {
        _mutex.lock();  //加锁
    }
    ~MutexLockGuard() {
        _mutex.unlock();    //解锁
    }

private:
    //MutexLock互斥锁类不可复制，所以只能是引用或指针类型
    MutexLock& _mutex;
};
#endif      //_MUTEXLOCKGUARD_HPP

