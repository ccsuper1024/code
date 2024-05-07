//
// Created by chenchao on 24-5-7.
//
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono;

int main()
{
    std::future<int> future = std::async(std::launch::async, [](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 8;
    });

    std::cout << "waiting...\n";
    std::future_status status;

    do{
        switch (status = future.wait_for(std::chrono::seconds(1)); status) {
            case std::future_status::deferred:
                std::cout << "deferred!\n";
                break;
            case std::future_status::timeout:
                std::cout << "timeout!\n";
                break;
            case std::future_status::ready:
                std::cout << "ready!\n";
                break;
        }
    } while (status != std::future_status::ready);

    std::cout << "result is " << future.get() << '\n';
}
