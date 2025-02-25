//如果一个线程已经对互斥锁加锁了，那么其他线程必须组成等待此线程解锁后才能获取锁
//同一线程对同一把锁重复加锁会造成死锁
//发生错误时抛出 std::system_error ，包括来自底层操作系统的错误，这些错误会阻止
//锁满足其规范。在抛出任何异常的情况下，互斥锁不会被锁定

#include <iostream>
#include <mutex>
#include <thread>

int g_num = 0;  //protected by g_num_mutex
std::mutex g_num_mutex;

void slow_increment(int id) {
    for (int i = 0; i < 3; ++i) {
        g_num_mutex.lock();
        ++g_num;
        fprintf(stdout, "id: %d, g_num: %d\n", id, g_num);
        g_num_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(234));
    }
}

int main(void) {
    std::thread t1(slow_increment, 0);
    std::thread t2(slow_increment, 1);
    t1.join();
    t2.join();
}