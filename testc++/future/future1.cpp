//
// Created by chenchao on 24-5-7.
//

#include <future>
#include <iostream>

int main()
{
    std::promise<void> p ;
    std::future<void> f = p.get_future();

    std::cout << std::boolalpha;

    std::cout << f.valid() << '\n';
    p.set_value();              //原子的设置promise的值
    std::cout << f.valid() << '\n';
    f.get();                    //调用std::future的wait() ,然后获取值
    std::cout << f.valid() << '\n';
}