/*  @file @brief @author chenchao 
    @brief 
*/
#ifndef  _THREAD_HPP
#define  _THREAD_HPP
#include<pthread.h>
#include<functional>
#include<utility>
class Thread
{
    using ThreadCallback = std::function<void()>;   //函数对象
public:
/* @file @brief @pram*/
    //默认无参构造函数,即运行隐式构造无参对象
    //无参中还有一种是参数都是默认参数，这也是一种无参构造，只是还兼具其他的构造
    Thread(ThreadCallback&& run);
    ~Thread();

    //线程运行与停止
    void start();
    void join();
private:
    static void* threadFunc(void* arg);
private:
    pthread_t _tid;
    bool _isRunning;
    ThreadCallback _run;
};
#endif      //_THREAD_HPP

