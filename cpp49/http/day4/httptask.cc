//使用workflow实现一个http任务
//标准c库
#include <signal.h>
//标准c++库
#include <iostream>
//第三方库
#include <workflow/WFFacilities.h>
#include <workflow/WFTaskFactory.h>

using std::cout;
using std::endl;

static WFFacilities::WaitGroup waitgroup(1);

void sighandler(int num){
    printf("sig number %d is comming\n",num);
    waitgroup.done();
}
void httpCallback(WFHttpTask* httptask){
    //当回调函数被运行时，已经完成了数据的接收和转发
    cout << "httpCallback is running" << endl;
}

int main()
{
    //注册信号
    signal(SIGINT,sighandler);

    //创建httptask任务,通过工厂函数
    std::string url = "http://www.baidu.com";
    WFHttpTask* httpTask = WFTaskFactory::create_http_task(url,1,1,httpCallback);

    //设置任务的属性
    auto req = httpTask->get_req();
    req->add_header_pair("User-Agent","workflow http client");

    //将任务交给框架来执行，是由框架来调度
    httpTask->start();
    //阻塞等待
    waitgroup.wait();
    return 0;
}

