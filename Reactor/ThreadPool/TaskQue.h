//
// Created by chenchao on 24-5-7.
//
#ifndef REACTOR_TASKQUE_H
#define REACTOR_TASKQUE_H
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class TaskQue
{
    using ElemType = std::function<void()>;
public:
    explicit TaskQue(size_t queSize);
    ~TaskQue();

    //任务队列判空与判满
    bool empty() const;
    bool full() const;

    //添加任务与弹出任务
    void push(ElemType &&value);
    ElemType pop();

    void wakeup();
private:
    int _queSize;                            //任务队列大小
    std::queue<ElemType> _que;              //任务队列
    std::mutex _mtx;                        //锁
    std::condition_variable _notEmpty;      //当队列为空时的条件变量
    std::condition_variable _notFull;       //当队列满时的条件变量
    bool  _flag;                            //让工作线程唤醒时，可以退出
};


#endif //REACTOR_TASKQUE_H
