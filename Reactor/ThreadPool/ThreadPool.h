//
// Created by chenchao on 24-5-7.
//

#ifndef REACTOR_THREADPOOL_H
#define REACTOR_THREADPOOL_H
#include "TaskQue.h"
#include <vector>
#include <memory>
#include <thread>

class ThreadPool
{
    using Task = std::function<void()>;
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task&& task);
private:
    Task getTask();
    void doTask();
private:
    size_t _threadNum;
    size_t _queSize;
    std::vector<std::unique_ptr<std::thread>> _threadPool;
    TaskQue _taskQue;
    bool _isExit;                               //
};

#endif //REACTOR_THREADPOOL_H
