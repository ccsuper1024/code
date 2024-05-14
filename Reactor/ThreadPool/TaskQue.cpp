//
// Created by chenchao on 24-5-7.
//

#include "TaskQue.h"
TaskQue::TaskQue(size_t queSize)
:_queSize(queSize)
,_que()
,_mtx()
,_notEmpty()
,_notFull()
,_flag(true)
{
}
TaskQue::~TaskQue()
{
}

//任务队列判空与判满
bool TaskQue::empty() const
{
    return 0 == _que.size();
}
bool TaskQue::full() const
{
    return static_cast<long unsigned int>(_queSize) == _que.size();
}


//添加任务与弹出任务
void TaskQue::push(ElemType &&value)
{
    //加锁
    std::unique_lock<std::mutex> uk(_mtx);  //基于RAII机制的锁管理对象
    //判断队列是否为空，再等待。防止虚假唤醒
    while(full()) {
        _notFull.wait(uk);
    }
    //_notFull.wait(uk, [this](){   return !this->full();});


    //新任务入队
    _que.push(std::move(value));

    //生产者生产任务后，通知消费者
    _notEmpty.notify_one();
}

//如过 using ElemType = std::function<void()>;在类内私有访问区域，则此处的ElemType会出错
TaskQue::ElemType TaskQue::pop()
{
    //加锁。std::unique_lock 是基于RAII机制的独占式加锁方式
    std::unique_lock<std::mutex> uk(_mtx);
    //_notEmpty.wait(uk, [this](){ return !this->empty(); });
    //循环判断，防止虚假唤醒
    while(empty()) {
        _notEmpty.wait(uk);
    }

    //弹出任务
    if(_flag) {
        //取任务
        ElemType task = _que.front();
        _que.pop();

        //唤醒生产者线程，生产任务
        _notFull.notify_one();
        return task;
    } else {
        return nullptr;
    }
}

void TaskQue::wakeup()
{
    _flag = false;
    _notEmpty.notify_all();
}
