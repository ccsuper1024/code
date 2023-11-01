/* @file  @brief */
#include "HeapTimer.hpp"
/* @brief @param @param   @return */

void hello()
{
    std::cout << "hello" << std::endl;
}
int main()
{
    HeapTimer ht;   //不需要加括号，不然容易被编译器误解为一个函数名。构造函数不加explicit都是默认调用的
    ht.Add(1,5,hello);
    ht.DoWork(1);
    return 0;
}

