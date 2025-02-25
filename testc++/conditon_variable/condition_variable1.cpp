/*
std::condition_variable 是一个和std::mutex一起使用的同步原语(会陷入内核态)。用来阻塞1个或多个线程(生产者or消费者)直到另一个线程(消费者or生产者)
修改了共享变量并通知条件变量
想要修改共享变量的线程必须：1.获取std::mutex互斥量(一般用std::lock_guard)
                       2.当拥有锁时修改共享数据
                       3.在条件变量上调用notify_one or notify_all来通知等待在条件变量上队列中的线程。(可以在释放锁之后调用)
即使共享变量是原子的，也必须在拥有互斥量的同时对其进行修改，才能正确地将修改发布到等待线程。

*/
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <thread>

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread() {
    //wait until main() sends data
    //std::unique_lock 是基于RAII机制的互斥量包装器，初始化即加锁。（在没有使用std::defer_lock 策略的情况下）
    std::unique_lock lk(m);
    cv.wait(lk, []{return  ready;});    //隐式捕获   相当于while(条件) { std::condition_variable.wait();}
    //在Linux平台上。pthread_cond_wait()具体过程如下:
//    1.检查是否加锁
//    2.把当前线程挂到条件变量的等待队列上
//    3.原子的解锁并等待
//
//    4.被notify_one or notify_all 唤醒后 竞争锁的所有权
//    在持有锁的状态下运行后续的代码
//    解锁

    //after the wait , we own the lock
    std::cout << "Worker thread is processing data\n";
    data += " after processing";

    //send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed\n";

    //manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block agian(see notify_one for detial)
    lk.unlock();
    cv.notify_one();
}

int main()
{
    std::thread worker(worker_thread);

    data = "Example data";
    //send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    //wake up the worker thread
    cv.notify_one();

    //wait for the worker
    {
        std::unique_lock lk(m);
        cv.wait(lk, []{ return processed; });
        /*
        相当于while([]{ return processed; }) {
            cv.wait(lk);
        }
        */
    }
    std::cout <<   "Back in main(), data = " << data << '\n';

    worker.join();
}