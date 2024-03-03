#include<iostream>
#include<cstdio>
int main(void) {
    char str[] = "abcd";
    std::cout << "sizeof(str): " << sizeof(str) << std::endl;
    std::cout << "str = " << &str << std::endl;
    std::cout << "the address of last str is: " << &str[4] << "str[4] = " <<str[4] << std::endl;
    std::cout << "&str + 1 = " << &str + 1 << std::endl;   
    std::cout << "str + 1 = " << str + 1 << std::endl;
    //std::cout << "&(str + 1): " << &(str + 1) << std::endl;
} 