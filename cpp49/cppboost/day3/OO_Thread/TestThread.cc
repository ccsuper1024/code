/* @author chenchao @date @file @brief*/
#include "Thread.h"
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

class Task
:public Thread   //面向对象，要使用就要先继承，再扩展
{
public:
    //覆盖抽象基类中的纯虚函数接口
    virtual void run() override final {
        cout << "I am running " << endl;
    }
};

/* @brief @pram @return */
void test(){
    Task myTask;
    myTask.start();
    myTask.join();
}
int main()
{
    test();
    return 0;
}

