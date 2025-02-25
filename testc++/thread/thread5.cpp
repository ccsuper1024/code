//use native_handle to enable realtime scheduling of C++ threads on a POSIX system
// 执行当前程序需要root权限
#include<chrono>
#include<iostream>
#include<cstring>
#include<mutex>
#include<pthread.h>
#include<thread>

std::mutex iomutex; //全局锁
void func(int num) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // struct sched_param
    // {
    //   int sched_priority;
    // };
    sched_param sch;
    int policy;
    /*
    The pthread_self() function returns the ID of the calling thread. This is the same value that is 
    returned in *thread in the pthread_create(3) call that created this thread.
    */
    pthread_getschedparam(pthread_self(), &policy, &sch);
    std::lock_guard<std::mutex> lk(iomutex);
    std::cout << "Thread " << num << " is executing at priority "
        << sch.sched_priority  << std::endl;
}

int main(void) {
    std::thread t1(func, 1), t2(func, 2);   //接受的可调用对象非空，则std::thread对象会自动启动
    sched_param sch;
    int policy;
    pthread_getschedparam(t1.native_handle(),&policy, &sch);
    sch.sched_priority = 20;
    if(pthread_setschedparam(t1.native_handle(), SCHED_FIFO,&sch)) {
        std::cout << "Failed to setschedparam: " << std::strerror(errno) << std::endl;
    }

    t1.join();
    t2.join();
}