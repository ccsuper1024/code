#include <iostream>
#include <functional>
#include <future>

using std::placeholders::_1;
using std::placeholders::_2;

void dosomething(int x, int y)
{
    std::cout << "do something:x = " << x << "y = " << y << std::endl;
}

int main()
{
    int x = 2;
    int y = 5;
    std::function<void(int,int)> fun = std::bind(dosomething, _2,_1);
    std::cout << "Hello world" << std::endl;
    auto func = std::async(dosomething, 5, 6);
    return 0;
}

