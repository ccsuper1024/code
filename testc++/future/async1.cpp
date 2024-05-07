//
// Created by chenchao on 24-5-7.
//
#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <vector>

std::mutex m;

struct Task
{
    void foo(int i, const std::string& str)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << str << ' ' << i << '\n';
    }

    void bar(const std::string& str)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << str << '\n';
    }

    int operator()(int i)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << i << '\n';
        return i + 10;
    }
};
//函数模板
template<typename RandomIt>
int parallel_sum(RandomIt beg, RandomIt end)
{
    auto len = end - beg;
    if (len < 1000)     //len的类型与整型字面量可比较
    {
        return std::accumulate(beg, end, 0);
    }

    RandomIt mid = beg + len / 2;
    //异步递归调用
    //handle是一个std::future对象
    auto handle = std::async(std::launch::async, parallel_sum<RandomIt>, mid, end);

    //递归
    int sum = parallel_sum(beg,mid);
    return sum + handle.get();
}

int main()
{
    std::vector<int> v(10000, 1);   //动态数组中有10000个1
    std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';

    Task task;
    //Calls (&task)->foo(42, "hello") with default polocy
    //may print "Hello 42" concurrently or defer execution
    //默认执行策略是立即执行
    auto a1 = std::async(&Task::foo, &task, 42, "Hello");

    //Calls task.bar("world!") with deferred policy
    //prints "world" when a2.get() or a2.wait() is called
    //惰性求值
    auto a2 = std::async(std::launch::deferred, &Task::bar,task , "world");

    //Calls task()(43); with async policy
    //print "43" concurrently
    auto a3 = std::async(std::launch::async, Task(), 43);
    a2.wait();                              // prints "world!"
    std::cout << a3.get() << '\n';          // prints "53"
}   //if a1 is not done at this point, destructor of a1 prints "Hello 42" here
/*
最终结果可能为
The sum is 10000
world
Hello 42
43
53

或:
The sum is 10000
world
43
53
Hello 42
等几种情况
总之，"The sum is 10000"必定是第一的  然后43 必定在53之前。而world 和 Hello 42 则是随机的
*/