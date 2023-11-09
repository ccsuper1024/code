/*@file 4_redistask.cc  @brief redistask是redis的客户端程序。作为发出请求，获取响应的一方*/
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include <workflow/WFTaskFactory.h>
#include <workflow/RedisMessage.h>
#include <workflow/WFFacilities.h>
using std::vector;
using std::endl;
using std::cout;
using std::cerr;
using std::cin;
using std::string;
static WFFacilities::WaitGroup waitgroup(1);

void sighandler(int signum){
    cout << "signum " << signum << "is comming" << endl;
    waitgroup.done();
}

void redisCallback(WFRedisTask* redisTask){
    cout<< "redisCallback is running" << endl;
    //1.错误的检测
    int state = redisTask->get_state();
    int error = redisTask->get_error();

    switch(state){
        case WFT_STATE_DNS_ERROR:
            cerr << "dns error: " << gai_strerror(error) << endl;
            break;
        case WFT_STATE_SYS_ERROR:
            cerr << "system error: " << strerror(error) << endl;
            break;
        case WFT_STATE_SUCCESS:
            cout << "task sucess" << endl;
            break;
    }
    if(state != WFT_STATE_SUCCESS){
        cerr << "error occurs" << endl;
    }

    //获取请求信息
    auto req = redisTask->get_req();
    string command;
    vector<string> params;
    req->get_command(command);
    req->get_params(params);
    cout << "command: ";
    for(auto& e:params){
        cout << e << " " ;
    }
    cout << endl;

    //获取响应信息
    auto resp = redisTask->get_resp();
    protocol::RedisValue result;
    resp->get_result(result); //const成员函数

    //遍历RedisValue的数据
    if(result.is_string()){
        cout << "value is string, value: " 
            << result.string_value() << endl;
    }
    

}
/* @brief @pram @return */
void test(){
    //注册信号
    signal(SIGINT,sighandler);

    //创建任务
    std::string redisUrl = "redis://127.0.0.1:6379";
    WFRedisTask* redisTask = WFTaskFactory::create_redis_task(redisUrl,1
        ,redisCallback);
    //设置任务属性
    auto redisReq = redisTask->get_req();
    redisReq->set_request("set",{"lwh","123"}); //set {"lwh","123"}
    //启动框架
    redisTask->start();
    //阻塞等待
    waitgroup.wait();
}
int main()
{
    test();
    return 0;
}

