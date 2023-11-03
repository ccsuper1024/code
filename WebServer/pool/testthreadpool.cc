/* @file  @brief */
#include <iostream>
#include "./threadpool.h"


using std::cout;
using std::cin;
using std::endl;
using std::cerr;

void task(){
    cout << "hello" << endl;
}
/* @brief @param @param   @return */
int main()
{
    ThreadPool threadpool(8);
    threadpool.AddTask(std::move(task));
    return 0;
}

