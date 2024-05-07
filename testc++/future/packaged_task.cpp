//
// Created by chenchao on 24-5-7.
//
#include <iostream>
#include <cmath>
#include <functional>
#include <future>
#include <thread>

//unique function to avoid disambiguating the std::pow overload set
//避免消除 std::pow 重载集歧义的独特函数
int f(int x, int y) {   return std::pow(x, y);  }

void task_lambda()
{
    std::packaged_task<int(int, int)> task([](int a, int b){
        return std::pow(a, b);          //计算底数a的b次方值
    });
    std::future<int> result = task.get_future();
    task(2, 9);

    std::cout << "task_lambda:\t" << result.get() << '\n';  //调用wait()等待，直到共享状态就绪
}

void task_bind()
{
    std::packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> result = task.get_future();

    task();

    std::cout << "task_bind:\t" << result.get() << '\n';
}

void task_thread()
{
    std::packaged_task<int(int, int)> task(f);
    std::future<int> result  = task.get_future();

    std::thread task_td(std::move(task), 2, 10);
    task_td.join();

    std::cout << "task_thread:\t" << result.get() << '\n';
}

int main()
{
    task_bind();
    task_lambda();
    task_thread();
}