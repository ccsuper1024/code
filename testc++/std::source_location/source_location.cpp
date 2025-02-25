/*  @file @author chenchao @brief */
#include <iostream>
#include <source_location>
#include <string_view>
 
void log(const std::string_view message,
         const std::source_location location =
               std::source_location::current())
{
    std::clog << "file: "
              << location.file_name() << '('
              << location.line() << ':'
              << location.column() << ") `"
              << location.function_name() << "`: "
              << message << '\n';
}
 
template<typename T>
void fun(T x)
{
    log(x); // line 20
}
 
int main(int, char*[])
{
    log("Hello world!"); // line 25
    fun("Hello C++20!");
}
