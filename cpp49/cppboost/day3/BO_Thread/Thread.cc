/*@file @brief @author chenchao @brief */
#include "Thread.h"
#include <stdio.h>
#include <iostream>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

/* @brief @pram @return */
Thread::Thread(ThreadCallback &&run)
:_tid(0)
,_isRunning(false)
,_run(std::move(run))
{
    fprintf(stdout,"Thread()\n");
}
Thread::~Thread() {
    fprintf(stderr,"~Thread()\n");
}

// 线程运行与停止
void Thread::start() {
    int ret = pthread_create(&_tid,nullptr,threadFunc,this);
    if(ret != 0) {
        perror("pthread_create\n");
        exit(EXIT_FAILURE);
    }
    _isRunning = true;
}
void Thread::join() {
    if(_isRunning) {
        int ret = pthread_join(_tid,nullptr);
        if(ret != 0) {
            perror("pthread_create\n");
            exit(EXIT_FAILURE);
        }
        _isRunning = false;
    }
}

void *Thread::threadFunc(void *arg) {
    Thread* t = static_cast<Thread*>(arg);
    if(t) {
        t->_run();
    } else {
        fprintf(stderr,"the thread'addres is null\n");
    }
    pthread_exit(nullptr);
}