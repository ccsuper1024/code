/*@file @brief*/
#include <signal.h>
#include <iostream>
#include <workflow/WFFacilities.h>


using std::endl;
using std::cout;
using std::cerr;
using std::cin;

static WFFacilities::WaitGroup waitGroup(2);    //等待  初始化一个静态对象

void sighandler(int num){
    printf("sig number %d is comming \n",num);
    waitGroup.done();
}

void test(){
    //注册信号
    signal(SIGINT,sighandler);

    waitGroup.wait();   //阻塞等待

}
int main()
{
    test();
    return 0;
}

