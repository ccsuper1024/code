//
// Created by chenchao on 24-5-12.
//
#include <iostream>
#include <vector>
class tmp
{};
int main()
{
    std::cout << "sizeof(tmp) = " << sizeof(tmp) << std::endl;
    std::vector<int> vec{1,2,3,4};
    std::cout << "vec's size = " << vec.size() << std::endl;
    std::cout << "vec's capacity = " << vec.capacity() << std::endl;
    vec.resize(2);
    std::cout << "vec's size = " << vec.size() << std::endl;
    std::cout << "vec's capacity = " << vec.capacity() << std::endl;
}
