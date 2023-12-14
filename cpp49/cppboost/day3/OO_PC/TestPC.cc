/*  @file @brief @author chenchao 
 *  @brief 测试面对对象实现的生产者和消费者模型。试着使用googletest
 *
 *  */
//#include <gtest>
#include "Producer.h"
#include "Consumer.h"
#include <stdio.h>
#include <iostream>
#include <memory>

using std::unique_ptr;
using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
void test(){
    TaskQueue taskque(4);
    //消费者和生产者共用一个任务队列
    unique_ptr<Thread> prod1(new Producer(taskque));     //基类指针指向派生类对象
    unique_ptr<Thread> prod2(new Producer(taskque));
    unique_ptr<Thread> prod3(new Producer(taskque));

    unique_ptr<Thread> con1(new Consumer(taskque));
    unique_ptr<Thread> con2(new Consumer(taskque));

    //将生产者和消费者运作起来
    prod1->start();
    prod2->start();
    prod3->start();
    con2->start();
    con2->start();

    prod1->join();
    prod2->join();
    prod3->join();
    con2->join();
    con2->join();  
}
int main()
{
    test();
    return 0;
}

