/*  @file @brief @author chenchao @brief */
#include "ThreadPool.h"

#include <unistd.h> //for sleep()
#include <iostream>

/* @brief @pram @return */
ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
:_threadNum(threadNum)
,_queSize(queSize)
,_threadpool()
,_taskque(_queSize)
,_isExit(false)
{
    _threadpool.reserve(_threadNum);    //预留存储空间
    fprintf(stdout,"ThreadPool(size_t threadNum, size_t queSize)\n");
}
ThreadPool::~ThreadPool(){
    fprintf(stdout,"~ThreadPool()\n");
}
void ThreadPool::start() {
    //创建出所有的工作线程，并且将其放在vector中存储起来
    for(size_t idx = 0; idx < _threadNum; ++idx) {
        unique_ptr<Thread> th(new Thread(std::bind(&ThreadPool::doTask,this)));
        _threadpool.push_back(std::move(th));
    }
    //遍历线程池，启动所有线程
    for(auto& e:_threadpool) {
        e->start();
    }
}
void ThreadPool::stop() {   //此处最原始的代码见cpp49视频
    //只要任务队列中的任务没有执行完毕，就不能让工作线程退出
    while(!_taskque.empty()) {
        sleep(1);
    }
    _isExit = true; //虽然无法保证工作线程执行doTask的速度要慢于这个变量的修改，但可以在因为执行doTask()中的getTask()而陷入睡眠的线程被notifyAll()唤醒
    _taskque.wakeup();  //唤醒所有等待在notEmpty条件变量上的线程，即消费者线程，即工作线程

    //遍历线程池，停止所有工作线程
    for(auto&e :_threadpool) {
        e->join();
    }
}
//添加任务，相当于在做生产者的角色
void ThreadPool::addTask(Task&& task) {
    if(task) {
        _taskque.push(std::move(task));    //将任务加入任务队列
    }
}
//
ThreadPool::Task ThreadPool::getTask() {
    return _taskque.pop();
}
void ThreadPool::doTask() {
    //线程池中的工作线程调用doTask函数后，会执行此函数。那么在此函数中就需要判定
    //线程池中是否有任务。即通过getTask();再取执行具体的使用者给代码，即执行Task
    //,用std::bind+std::function来表现多态。
    //而此时执行任务的速率会影响最后的结果：如果执行任务的速率比较慢，那么就有足够的
    //时间让标志位_isExit设置为true。那么子线程就不会进入到doTask中的循环。那么如果
    //执行process的速率非常快，快到isExit()还没有来得及设置为true就已经进入到了
    //doTask,也就是while循环中，但是再执行getTask时，队列中又没有任务，所以工作
    //线程就会一直在_notEmpty条件变量上睡眠,这样，当主线程调用stop()后，线程的join
    //函数就永远等不到线程终止了。因为没有人去唤醒这些线程。所以我们需要notifyAll
    //让主线程在stop函数中调用notifyAll函数去唤醒这些睡眠的线程
    //
    //如果主线程不是在void test()中调用，会碰到线程无法退出的问题吗？毕竟return语句
    //相当于exit()函数，可以从进程中退出，直接终止整个进程，而进程中主线程的子线程
    //也会因为主线程死亡而死亡吧？？
    //
    //只要线程池一直不退出，就让工作线程一直去拿任务并且执行
    while(!_isExit) {
        //获取任务
        Task pTask = getTask();
        if(pTask) {
            //执行具体的任务
            pTask();
        }
    }
}