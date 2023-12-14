/*@file @brief @author chenchao @brief */
#include "Thread.h"
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

void myTask() {
    cout << "I am running \n" << endl;
}
void add(int x,int y) {
    //c语言的输出库函数，printf和fprintf都有锁，可以保证线程安全
    //但是这些输出函数都是用户态下的，会在加锁的情况下先把数据推入用户态下的缓冲区中
    //等当前线程结束后才会推送到内核，进而显示到屏幕
    fprintf(stdout,"x + y = %d\n",x+y);
    //printf("x + y = %d",x+y);
}
/* @brief @pram @return */
void test(){
    Thread t(myTask);
    t.start();
    

    int x = 5;
    Thread t1(std::bind(&add,2,x));
    t1.start();

    t.join();
    t1.join();
}
int main()
{
    test();
    return 0;
}

