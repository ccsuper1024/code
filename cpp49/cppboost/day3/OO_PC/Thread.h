/* @author chenchao @date @file 
    @brief 使用面对对象的方法设计线程类
*/
#ifndef  _THREAD_HPP
#define  _THREAD_HPP
#include <pthread.h>

//定义一个抽象类
class Thread
{
public:
/* @file @brief @pram*/
    Thread();
    virtual ~Thread();

    //线程停止与运行的函数
    void start();
    void join();
private:
    //因为pthread_create()中的第三个参数形式不可变，所以要把thread_func()定义为
    //一个静态函数，避免非静态函数的隐含的第一个参数：this指针。
    static void* threadfunc(void * arg);

    //纯虚函数
    //线程执行的任务
    virtual void run()=0;

private:
    pthread_t _tid;     //线程ID
    bool _isRunning;    //标志是否运行

};
#endif      //_THREAD_HPP

