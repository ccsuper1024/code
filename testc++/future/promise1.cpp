//
// Created by chenchao on 24-5-6.
//
#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise
                )
{
    int sum = std::accumulate(first, last , 0);
    //set_value()会原子的设置值
    accumulate_promise.set_value(sum);                  //notify future
}

void do_work(std::promise<void> barrier)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //set_value()会原子的设置值
    /*
    该操作的行为就像 set_value、set_exception、set_value_at_thread_exit 和 set_exception_at_thread_exit i
    在更新 Promise 对象时获取与 Promise 对象关联的单个互斥锁。
    调用此函数不会与调用 get_future 引入数据竞争（因此它们不需要彼此同步）。
    */
    //set_value()会原子的设置值
    barrier.set_value();
}

int main()
{
    //Demonstrate using promise<int> to transmit a result between threads
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise));

    //future::get() will wait until the future has a valid result and retrieves it.
    //calling wait() before get() is not needed
    //accumulate_future.wait(); //wait for thread completion
    std::cout << "result = "<< accumulate_future.get() << '\n';
    work_thread.join();                                         //wait for thread completion

    //Demostrate using promise<void> to signal state between threads
    std::promise<void> barrier;
    std::future<void> barrier_future = barrier.get_future();
    std::thread new_work_thread(do_work, std::move(barrier));
    barrier_future.wait();
    std::cout << "promise<void> gone" << std::endl;
    new_work_thread.join();
}