/*  @file @brief @author chenchao @brief */
#include "TaskQueue.h"

#include <iostream>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

/* @brief @pram @return */
TaskQueue::TaskQueue(size_t queSize)
:_queSize(queSize)
,_que()
,_mutex()   
,_notEmpty(_mutex)
,_notFull(_mutex)
{
    fprintf(stdout,"TaskQueue(size_t queSize)\n");
}
TaskQueue::~TaskQueue() {
    fprintf(stdout,"~TaskQueue()\n");
}
void TaskQueue::push(const int &value) {// 万能引用。可以接受，const值，左值，右值
    //由于会出现锁的争用，所以要加锁
    //1.先加锁
    _mutex.lock();
    //2.判满
    if(full()) {
        //如果任务队列是满的，则当前动作要阻塞等待
        //阻塞等待在notFull的条件变量中
        //即生产者
        _notFull.wait();
    }
    //3.如果不满，则存任务
    _que.push(value);

    //3.1.有了任务就可以通知等待在notEmpty条件变量上的线程
    _notEmpty.notify();

    //释放锁
    _mutex.unlock();
}
int TaskQueue::pop() {
    //1.加锁
    _mutex.lock();
    //2.判空
    if(empty()) {
        //则等待在条件变量_notEmpty的等待队列中.即消费者
        _notEmpty.wait();
    }
    //3.任务队列若不空，则从队列中取一个任务
    int tmp = _que.front();
    _que.pop();
    //3.1 通知等待在条件变量_notFull上的线程
    _notFull.notify();  //至少唤醒一个线程
    //4.解锁
    _mutex.unlock();

    return tmp;
}

// 被外部和内部调用
bool TaskQueue::empty() {
    return _que.size() == 0;
}
bool TaskQueue::full() {
    return _que.size() == _queSize;
}