/*  @file @brief @author chenchao @brief */
#ifndef  _MUTEX_HPP
#define  _MUTEX_HPP
#include <pthread.h>
#include "NoCopyable.h"

class MutexLock
:public NoCopyable
{
public:
/* @file @brief @pram*/
    MutexLock();
    ~MutexLock();

    //加解锁
    void lock();
    void unlock();
    void trylock();
    pthread_mutex_t* getMutexLockPtr();
private:
    pthread_mutex_t _mutex;
};
#endif  //_MUTEX_HPP

