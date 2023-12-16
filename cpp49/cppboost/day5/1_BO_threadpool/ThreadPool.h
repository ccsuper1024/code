/*  @file @brief @author chenchao @brief */
#ifndef  _THREADPOOL_HPP
#define  _THREADPOOL_HPP
#include "TaskQueue.h"
#include "Thread.h"
#include <functional>
#include <memory>
using std::vector;
using std::unique_ptr;  //独占式的智能指针

class ThreadPool
{
    using Task = std::function<void()>;
public:
/* @file @brief @pram*/
    ThreadPool(size_t threadNum = 4,size_t queSize = 10);
    ~ThreadPool();
    //线程池的启动
    void start();
    //线程池的退出
    void stop();
    //添加任务
    void addTask(Task&& task);
    int getTaskqueSize() {
        return _taskque.getsize();
    }
private:
    //获取任务
    ThreadPool::Task getTask();
    //线程池交给每个具体线程做的任务，封装成函数
    void doTask();
private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threadpool;
    TaskQueue _taskque;
    bool _isExit;
};
#endif      //_THREADPOOL_HPP

