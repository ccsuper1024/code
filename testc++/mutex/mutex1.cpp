//std::mutex 即不可复制也不可移动。
//std::mutx通常不会直接使用，而是被std::unique_lock,std::lock_guard,或者std::scoped_lock管理，来获得异常安全
//当前程序示例展现一个互斥锁是如何在两个线程之间被用来保护std::map
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>

/*
Because the default constructor is constexpr, static mutexes are initialized as part of 
static non-local initialization, before any dynamic non-local initialization begins.
This makes it safe to lock a mutex in a constructor of any static object
*/
std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;   //全局锁，默认初始化

void save_page(const std::string& url) {
    //simulate a long page fetch
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::string result = "fake content";

    std::lock_guard<std::mutex> guard(g_pages_mutex);   //基于范围的锁，出了当前程序返回会自动释放锁资源
    g_pages[url] = result;  
}

int main(void) {
    std::thread t1(save_page, "http://foo");
    std::thread t2(save_page, "http://bar");
    t1.join();
    t2.join();

    //safe to access g_pages without lock now, as the threads are joined.
    for(const auto& elem: g_pages) {
        std::cout << elem.first << "=>" << elem.second << "\n";
    }
}