/* @author chenchao @date @file @brief*/
#include "Thread.h"
#include <unistd.h>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
Thread::Thread()
:_tid()
,_isRunning(false)
{
    fprintf(stdout,"Thread()\n");
}
Thread::~Thread() {

}

//线程停止与运行的函数
void Thread::start() {
    /*
    threadfunc 作为成员函数时，第一个参数的位置会隐含一个this指针
    ，而pthread_create的第三个参数形态是固定的，所以需要消除this指针的影响
    就需要将threadfunc设置为静态的。同时第四个参数是传递给threadfunc参数的
    。这个参数应该是对象的this指针，用来运行this->run().否则，静态函数内无法
    调用成员函数，因为它没有this指针。
    */
    int ret = pthread_create(&_tid,nullptr,threadfunc,this);
    if(ret) {
        perror("pthread_create\n");
        exit(EXIT_FAILURE);
    }
    _isRunning = true;
}
void Thread::join() {
    if(_isRunning) {
        int ret = pthread_join(_tid,nullptr);
        if(ret) {
            perror("pthread_join\n");
            exit(EXIT_FAILURE);
        }
        _isRunning = false; 
    }
    
}
//因为pthread_create()中的第三个参数形式不可变，所以要把thread_func()定义为
//一个静态函数，避免非静态函数的隐含的第一个参数：this指针。
void* Thread::threadfunc(void * arg) {
    Thread* t = static_cast<Thread*>(arg);
    if(t) {
        t->run();       //c++的多态实现。调用了派生类的虚函数
    } else {
        fprintf(stderr,"threadfunc\n");
    }
    pthread_exit(nullptr);
}

//纯虚函数
//线程执行的任务
//void run()=0;  //纯虚函数在抽象类中不需要实现，只要继承类去实现即可

