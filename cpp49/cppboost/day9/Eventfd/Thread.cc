/* @author chenchao @date @file @brief*/
#include "Thread.h"
#include <stdio.h>      //posix规定printf是线程安全的，其内部有锁
#include <stdlib.h>     //exit()
/* @file @brief @pram*/
//防止发生类似Thread t = callback;的隐式转换
Thread::Thread(ThreadCallback&& cb)    //构造初始化类对象
:_tid(0)
,_isRunning(false)
,_cb(std::move(cb))
{
    printf("Thread(ThreadCallback&& cb)...\n");
}
   
//线程运行和停止的函数
void Thread::start(){
    //threadFunc作为成员函数的时候，在第一个参数的位置会隐含一个
    //this指针，而pthread_create的第三个参数形态是固定的，所以
    //需要消除this的影响，就需要将threadFunc设置为静态函数
    //int ret = pthread_create(&_tid,nullptr,threadFunc,nullptr);
    //pthread_create的第四个参数会传递给threadFunc的参数
    int ret = pthread_create(&_tid,nullptr,threadFunc,this);
    if(ret != 0){
        perror("pthread_create\n");
        exit(EXIT_FAILURE);
    }
    _isRunning = true;
}
void Thread::join(){
    //声明一个空指针，未赋值，当线程有返回值时，
    //pthread_join会给result赋值，从而在join函数栈上可以知道线程的返回值
    /*void * result;              
    int ret = pthread_join(_tid,&result);*/
    if(_isRunning){
        int ret = pthread_join(_tid,nullptr);
        if(ret != 0){
            perror("pthread_join\n");
        }
        _isRunning = false;
    }
    
    /*if(result != nullptr){
        printf("thread %lu return %d\n",_tid,*(int*)result);//先强转为整型的指针，再解引用
        delete result;
    }*/
    
}

//线程入口函数
void* Thread::threadFunc(void* arg){
    //pthread_create通过第四个参数传递Thread对象的地址给threadFunc
    Thread* thread = static_cast<Thread*>(arg);
    if(thread)  //当thread不为空
    {
        //int*result = new int(0);
        //*result = thread->_cb();      //在线程中调用回调函数,并返回值
        thread->_cb();  
        //pthread_exit((void*)result);
    }else{
        printf("nothing to callback");
        //int*result = new int(1);
        //pthread_exit((void*)result);
    }
    //因为pthread_exit()传递的是一个指针，而线程内的数据在线程退出后都会销毁。
    //只有进程的全局数据和堆上的数据才可以在线程退出时不被销毁，所以应该传递的
    //是堆上数据的指针

    pthread_exit(nullptr);
}

