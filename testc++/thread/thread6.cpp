//yield() 让操作系统重新调度线程，允许其他线程先运行，当前线程重新就绪
//比如说你的线程需要等待某个操作完成，如果直接用一个循环不断判断这个操作是否会完成，会使得这个线程占满CPU时间
//，这会造成资源浪费。这时候你可以判断一次操作是否完成，如果没有完成就主动调用yield()交出时间片，等下一次时间
//片轮转轮到当前线程后，再进行判断，这样这个线程的CPU占用时间就会大大降低。适用于那些耗时多且不紧急的事件
//不过线程的确切行为是依赖于平台的，windows下和Linux下是不同的。甚至不同的Linux版本也会不同。比如FIFO类型
//调度器的Linux和时间轮转多任务抢占式的Linux的表现是不同的。

#include <chrono>
#include <iostream>
#include <thread>

//"busy sleep " while suggesting that other threads run for a small amount of time
void little_sleep(std::chrono::microseconds us) {
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;
    do{
        std::this_thread::yield();
    }while(std::chrono::high_resolution_clock::now() < end);
}

int main(void) {
    auto start = std::chrono::high_resolution_clock::now();
    little_sleep(std::chrono::microseconds(100));

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    std::cout << "waited for "
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
              << " microseconds\n";
}