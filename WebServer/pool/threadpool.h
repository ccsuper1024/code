/* @file threadpool.h
 * @brief 
 */
#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>
#include <cassert>

using std::mutex;
using std::condition_variable;
using std::queue;
using std::thread;
using std::function;

class ThreadPool
{
public:
    //强制显式调用
    explicit ThreadPool(size_t threadCount = 8)
    :pool_(std::make_shared<struct Pool>())
    {
        assert(threadCount > 0);
        for(size_t i = 0; i < threadCount; i++){
            std::thread([=](){
                    std::unique_lock<mutex> locker(pool_->mtx_);
                    while(true) {
                        if(!pool_->tasks.empty()) {
                            auto task = std::move(pool_->tasks.front());
                            pool_->tasks.pop();
                            locker.unlock();
                            //回调函数
                            task();
                            locker.lock();
                        } 
                        else if(pool_->isClosed) break;
                        else pool_->cond.wait(locker);
                    }
                }).detach();    //分离线程
        }
    }
    ThreadPool()=default;
    ThreadPool(ThreadPool&&)=default;  //移动函数
    ~ThreadPool(){
        if(static_cast<bool>(pool_)){
            {
                std::lock_guard<mutex> lock(pool_->mtx_);
                pool_->isClosed = true;
            }
            pool_->cond.notify_one();
        }
    }

    template<class T>
        void AddTask(T&& task){
            {
                std::lock_guard<mutex> lock(pool_->mtx_);
                pool_->tasks.emplace(std::forward<T>(task));    //完美转发
            }
            pool_->cond.notify_one();
        }
private:
     struct Pool{
        mutex mtx_;
        condition_variable cond;
        bool isClosed;
        queue<function<void()>> tasks;      //任务队列
     };

     std::shared_ptr<struct Pool> pool_;
};

#endif  //THREADPOOL_HPP
