/*  @file @brief @author chenchao @brief */
#include "Condition.h"
#include <iostream>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

/* @brief @pram @return */
Condition::Condition(MutexLock& mutex) 
:_mutex(mutex)      //类成员的初始化顺序和声明顺序相同
//:_cond(PTHREAD_COND_INITIALIZER)
{
    int ret = pthread_cond_init(&_cond,nullptr);
    if(ret != 0) {
        perror("pthread_cond_init\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Condition(MutexLock& mutex)\n");
}
Condition::~Condition()
{
    int ret = pthread_cond_destroy(&_cond);
    if(ret != 0) {
        perror("pthread_cond_destroy\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"~Condition()\n");
}

// 至少唤醒一个线程
void Condition::notify() {
    int ret = pthread_cond_signal(&_cond);
    if(ret != 0) {
        perror("pthread_cond_signal\n");
        exit(EXIT_FAILURE);
    }
}
// 唤醒所有等待在条件变量中的线程
void Condition::notifyAll() {
    int ret = pthread_cond_broadcast(&_cond);
    if(ret != 0) {
        perror("pthread_cond_broadcast\n");
        exit(EXIT_FAILURE);
    }
}

// 阻塞等待
void Condition::wait() {
    int ret = pthread_cond_wait(&_cond,_mutex.getMutexLockPtr());
    if(ret != 0) {
        perror("pthread_cond_wait\n");
        exit(EXIT_FAILURE);
    }
}
