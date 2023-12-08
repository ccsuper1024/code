/* @author chenchao @date @file @brief*/
#include "Thread.h"
#include "EventFd.h"
#include <unistd.h>
#include <stdio.h>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

class Task
{
public:
    void process(){
        printf("Mytask is running...\n");
    }
};
/* @brief @pram @return */
void test(){
    Task mytask;    //隐式构造，调用默认构造函数
    EventFd efd(std::bind(&Task::process,&mytask)); //创建EventFd对象

    //线程执行EventFd::start以及handleRead，也就是执行read操作
    Thread t(std::bind(&EventFd::start,&efd));
    //启动线程
    t.start();
    //主线程继续于此处执行
    int cnt = 20;
    while(cnt-- > 0){//后缀自减，先返回，再减
        printf("cnt = %d\n",cnt);
        efd.wakeup();           //主线程执行wakeup，也就是write操作,子线程执行读操作
        sleep(1);
    }
    //终止eventfd
    efd.stop();
    //主线程等待子线程终止
    t.join();
}
int main()
{
    test();
    return 0;   
}

