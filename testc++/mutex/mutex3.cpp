//time_mutex是通过try_lock_for和try_lock_until()来尝试在超时时声明timed_mutex所有权的能力
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

std::mutex cout_mutex; // control access to std::cout
std::timed_mutex mutex;

void job(int id)
{
    std::ostringstream stream;

    for (int i = 0; i < 3; ++i)
    {
        if (mutex.try_lock_for(100ms))
        {
            stream << "success ";
            std::this_thread::sleep_for(100ms);
            mutex.unlock();
        }
        else
            stream << "failed ";

        std::this_thread::sleep_for(100ms);
    }

    std::lock_guard<std::mutex> lock{cout_mutex};
    std::cout << '[' << id << "] " << stream.str() << '\n';
}

int main()
{
    std::vector<std::thread> threads;
    for (int i{0}; i < 4; ++i)
        threads.emplace_back(job, i);

    for (auto& th : threads)
        th.join();
}