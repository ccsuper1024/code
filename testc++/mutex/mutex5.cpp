#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <syncstream>

class ThreadSafeCounter
{
public:
    ThreadSafeCounter()=default;

    //Multiple threads/readers can read the counter's value at the same time.
    unsigned int get() const {
        std::shared_lock lock(mutex_);  //
        return value_;                    //读操作
    }

    //Only one thread/writer can increment/write the counter's value.
    void increment() {
        std::unique_lock lock(mutex_);
        ++value_;                           //写操作
    }

    //Only one thread/writer can reset/write the counter's value
    void reset() {
        std::unique_lock lock(mutex_);
        value_ = 0;
    }
private:
    mutable std::shared_mutex mutex_;
    unsigned  int value_{};     //列表初始化
};

int main(void)
{
    ThreadSafeCounter counter;  //默认构造

    auto increment_and_print = [&counter]() {
        //i的值是线程内部的局部变量
        for(int i{}; i != 3; ++i) {
            counter.increment();    //写操作
            std::osyncstream(std::cout)
                << std::this_thread::get_id() << ' ' << counter.get()/*读操作*/ << '\n';
        }
    };
    std::thread t1(increment_and_print);
    std::thread t2(increment_and_print);

    t1.join();
    t2.join()
}
