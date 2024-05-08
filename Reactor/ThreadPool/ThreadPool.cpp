//
// Created by chenchao on 24-5-7.
//
#include <unistd.h>
#include "ThreadPool.h"
ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
:_threadNum(threadNum)
,_queSize(queSize)
,_threadPool()
,_taskQue(_queSize)
,_isExit(false)
{
    _threadPool.reserve(threadNum); //预留空间
}
ThreadPool::~ThreadPool()
{

}

void ThreadPool::start()
{
    //创建线程
    for(int i{}; i < _threadNum; ++i){
        std::unique_ptr<std::thread> uptrThread(new std::thread(&ThreadPool::doTask, this));
        _threadPool.push_back(std::move(uptrThread));
    }
}
void ThreadPool::stop()
{
    //只要任务队列中的任务没有执行完毕，就不能让线程池退出
    while(!_taskQue.empty()) {
        sleep(1);
    }

    _isExit = true;     //将标志位设为true
    _taskQue.wakeup();  //唤醒所有消费者线程来执行任务

    //遍历线程池，主线程等待子线程停止
    for(auto &th: _threadPool) {
        th->join();
    }
}

void ThreadPool::addTask(Task&& task){
    if(task) {  //当task任务不是空对象
        _taskQue.push(std::move(task));
    }

}
ThreadPool::Task ThreadPool::getTask(){
    return _taskQue.pop();      //返回值发生复制
}
void ThreadPool::doTask(){
    while (!_isExit) {
        //获取任务
        Task taskcb = getTask();
        if(taskcb) {
            //执行具体的任务
            taskcb();
        }
    }
}
