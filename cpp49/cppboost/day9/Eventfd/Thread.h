/* @author chenchao @date @file @brief*/
#ifndef  THREAD_HPP
#define  THREAD_HPP

#include <pthread.h>
#include <functional>

using std::function;
using std::bind;

class Thread
{
    //using ThreadCallback = function<int()>; //using 声明一个function对象，用于testthread.cc
    using ThreadCallback = function<void()>; 
public:
    /* @file @brief @pram*/
    explicit Thread(ThreadCallback&& cb);  //防止发生类似Thread t = callback;的隐式转换

    ~Thread()=default;
    //线程运行和停止的函数
    void start();
    void join();
private:
    //因为threadFunc函数是一个成员函数，其自带一个隐含的this指针，所以在传递
    //给pthread_create的第三个参数时会出现错误，所以要将此函数定义为私有的静态函数
    static void* threadFunc(void* arg);
private:
    pthread_t _tid;     //线程id
    bool _isRunning;    //标识线程是否运行
    ThreadCallback _cb; //线程回调函数

};
#endif  //THREAD_HPP

