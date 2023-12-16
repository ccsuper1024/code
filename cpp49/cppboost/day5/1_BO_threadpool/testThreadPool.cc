/*  @file @brief @author chenchao 
 *  @brief 测试面向对象实现的线程池
 *
 *  */
//#include <gtest>
#include "ThreadPool.h"

#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;
class MyTask
{
public:
    void process(){
        //业务逻辑，应该是计算线程
        ::srand(::clock());
        int number = rand() % 100;
        //线程安全
        fprintf(stdout,"ThreadPool.number = %d\n",number);
    }
};

/* @brief @pram @return */
void test(){
    MyTask mytask; 
    ThreadPool threadpool(4,10);    //创建4个线程并陷入睡眠，任务队列为10

    threadpool.start();     //开启的工作线程就相当于消费者，此时陷入睡眠，因为任务队列为空，等待在notEmpty条件变量的等待队列上
    //将任务加入线程池
    int cnt = 20;
    while (cnt-- > 0) {
        //相当于生产者
        threadpool.addTask(std::bind(&MyTask::process,&mytask));  //接受一个临时对象，std::bind返回一个function对象
        fprintf(stdout,"cnt = %d\n",cnt);
    }
    threadpool.stop();
}
int main()
{
    test();
    return 0;
}

