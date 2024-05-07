#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v{3,-4,2,-8,15, 267};  //列表初始化会调用复制构造或者移动构造

    auto print = [](const int& n) {         //引用传递参数
            std::cout << n << ' ';
    };
    std::cout << "before:\t";
    std::for_each(v.cbegin(), v.cend(), print);
    std::cout << '\n';

    //increment elements in-palce
    std::for_each(v.begin(), v.end(),[](int& n) { n++; });  //引用传递参数

    std::cout << "after:\t";
    std::for_each(v.cbegin(), v.cend(), print);
    std::cout << '\n';

    struct Sum                          //嵌入类, 可调用对象
    {
        void operator()(int n) {    sum += n;   }
        int sum{0};
    };

    //invoke Sum::operator() for each element
    Sum s = std::for_each(v.cbegin(), v.cend(), Sum());     //调用了默认构造和合成的复制构造
    std::cout << "sum::\t" << s.sum << '\n';

}