/*  @file @brief @author chenchao @brief */
#include "TaskQueue.h"
#include "MutexLockGuard.h"
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
,_flag(true)    //任务队列一创造就应该可以运行工作线程了
{
    fprintf(stdout,"TaskQueue(size_t queSize)\n");
}
TaskQueue::~TaskQueue() {
    fprintf(stdout,"~TaskQueue()\n");
}

//添加元素到任务队列
void TaskQueue::push(ElemType&& value) {// 万能引用。可以接受，const值，左值，右值
    //由于会出现锁的争用，所以要加锁
    //1.先加锁
    //_mutex.lock();
    //使用RAII机制管理互斥锁的释放,锁的释放依赖于栈对象的生命期     存储期，链接性，作用域
    MutexLockGuard autoLock(_mutex);

    //2.判满,
    //为了防止wakeup()中的notEmpty.notifyAll虚假唤醒，造成大量的线程切换开销，
    //所以把if换成while，但这应该不会影响notFULL上的等待队列中的线程
    while(full()) {
        //如果任务队列是满的，则当前动作要阻塞等待
        //阻塞等待在notFull的条件变量中
        //即生产者
        _notFull.wait();
    }
    //3.如果不满，则存任务
    _que.push(std::move(value));

    //3.1.有了任务就可以通知等待在notEmpty条件变量上的线程，即消费者
    _notEmpty.notify();

    //释放锁
    //_mutex.unlock();
}
//从任务队列删除元素
TaskQueue::ElemType TaskQueue::pop() {
    //1.加锁
    //_mutex.lock();
    //使用RAII机制管理互斥锁的释放
    MutexLockGuard autoLock(_mutex);
    
    //2.判空，为了防止wakeup()中的notEmpty.notifyAll虚假唤醒，所以把if换成while
    while(empty() && _flag) {
        //则等待在条件变量_notEmpty的等待队列中.即消费者
        _notEmpty.wait();
    }
    if(_flag) {
        //3.任务队列若不空，则从队列中取一个任务
        ElemType tmp = _que.front();
        _que.pop();
        //3.1 通知等待在条件变量_notFull上的线程
        _notFull.notify();  //至少唤醒一个生产者线程
        return tmp;
    } else {
        return nullptr;
    }
    
    //4.解锁
    //_mutex.unlock();
}
//唤醒所有等待在非空条件变量下的线程。因为在ThreadPool类中，为了使得线程持续不断的
//完成任务时，所以当任务队列中还有任务，就不能让线程退出，即不能在主线程中调用线程
//的join()函数。但这会造成的一个问题是在该退出时如何保证所有的线程在该退出时退出
//即让所有线程苏醒
void TaskQueue::wakeup() {
    _flag = false;  //这就表示pop()中的等待空条件的线程不再阻塞了，即消费者线程不再有阻碍了
    _notEmpty.notifyAll();
}

// 被外部和内部调用
bool TaskQueue::empty() {
    return _que.size() == 0;
}
bool TaskQueue::full() {
    return _que.size() == _queSize;
}