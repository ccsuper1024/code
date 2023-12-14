/*  @file @brief @author chenchao @brief */
#include "MutexLock.h"
#include <iostream>

/* @brief @pram @return */
MutexLock::MutexLock()
//:_mutex(PTHREAD_MUTEX_INITIALIZER)  //静态初始化
{
    //POSIX线程中互斥锁的动态初始化
    int ret = pthread_mutex_init(&_mutex,nullptr);  //nullptr表示默认锁属性，即不可递归锁
    if(ret != 0) {
        perror("pthread_mutex_init\n");
        exit(EXIT_FAILURE);
    }
    //线程安全，不需要担心线程争用
    fprintf(stdout,"MutexLock()");
}
MutexLock::~MutexLock()
{
    int ret = pthread_mutex_destroy(&_mutex);
    if(ret != 0) {
        perror("pthread_mutex_destroy\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"~MutexLock()");
}

// 加解锁
void MutexLock::lock() {
    int ret = pthread_mutex_lock(&_mutex);
    if(ret != 0) {
        perror("pthread_mutex_lock\n");
        exit(EXIT_FAILURE);
    }
}
void MutexLock::unlock() {
    int ret = pthread_mutex_unlock(&_mutex);
    if(ret != 0) {
        perror("pthread_mutex_unlock\n");
        exit(EXIT_FAILURE);
    }
}
void MutexLock::trylock() {     //尝试一下加锁，若加锁成功则返回0，加锁不成功为-1
    int ret = pthread_mutex_trylock(&_mutex);
    if(ret != 0) {
        perror("pthread_mutex_trylock\n");
        exit(EXIT_FAILURE);
    }
}
pthread_mutex_t* MutexLock::getMutexLockPtr() {
    return &_mutex;
}
