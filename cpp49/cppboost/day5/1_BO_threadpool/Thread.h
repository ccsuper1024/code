/* @author chenchao @date @file 
    @brief 使用面对对象的方法设计线程类
*/
#ifndef  _THREAD_HPP
#define  _THREAD_HPP
#include <pthread.h>
#include "NoCopyable.h"
#include <functional>
//定义一个抽象类
class Thread
:public NoCopyable
{
    using ThreadCallback = std::function<void()>;
public:
/* @file @brief @pram*/
    Thread(ThreadCallback && cb);
    ~Thread();

    //线程停止与运行的函数
    void start();
    void join();
    //回调函数    
private:
    //因为pthread_create()中的第三个参数形式不可变，所以要把thread_func()定义为
    //一个静态函数，避免非静态函数的隐含的第一个参数：this指针。
    static void* threadfunc(void * arg);
private:
    pthread_t _tid;     //线程ID
    bool _isRunning;    //标志是否运行
    ThreadCallback _cb; //回调函数
};
#endif      //_THREAD_HPP

