#include <iostream>
#include <map>
#include <string>
#include <set>
//for human read style
#include <cxxabi.h>

int main(void)
{
    std::set<int> intset = {1,5,89,65};
    std::map<std::string, std::set<int>> freqMan = {{"Mr.chantele",{1,35,8,69}}};

    /*wrong example*/
    // std::cout << "freqMan::key_type = " << freqMan::key_type << std::endl;
    // 如何获取一个对象的运行时信息
    using Key_Type = decltype(freqMan)::key_type;
    
    int status;
    char* demangled_name = abi::__cxa_demangle(typeid(Key_Type).name(), 0, 0, &status);
    std::cout << "freqMan::key_type = " << (status == 0 ? demangled_name : typeid(Key_Type).name()) << std::endl;

    free(demangled_name);
    return 0;
}

