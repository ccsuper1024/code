/*@file @brief redis客户端程序，任务间传递数据的方式 3 */
//通过lambda表达式捕获变量来获得共享数据
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <workflow/WFFacilities.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/RedisMessage.h>

using std::string;
using std::vector;
using std::shared_ptr;
using std::endl;
using std::cout;
using std::cerr;
using std::cin;

static WFFacilities::WaitGroup waitGroup(1);
void sighandler(int signum){
    cout << "sig number" << signum << "is comming" << endl;
    waitGroup.done();
}

/* @brief @pram @return */
void test(){
    //注册信号
    signal(SIGINT,sighandler);
    //共享数据
    int *pnumber = new int(100);

    //创建任务
    string redisUrl = "redis://127.0.0.1:6379";
    WFRedisTask* redisTask = WFTaskFactory::create_redis_task(redisUrl,1,
    [pnumber](WFRedisTask* redisTask){
         cout << "redisCallback1 is running " << endl;
         cout << "pnumber: " << pnumber << endl;
         cout << "*pnumber: " << *pnumber << endl;

        //1.错误的检测
        int state = redisTask->get_state();
        int error = redisTask->get_error();

        switch(state) {
        case WFT_STATE_SYS_ERROR:
            printf("system error: %s\n", strerror(error)); break;
        case WFT_STATE_DNS_ERROR:
            printf("dns error: %s\n", gai_strerror(error)); break;
        case WFT_STATE_SUCCESS:
             break;
        }

        if(state != WFT_STATE_SUCCESS) {
            printf("error occurs!\n");
            return;
        }

        printf("task success\n");
        //2. 获取请求信息
        auto req = redisTask->get_req();
        std::string command;
        std::vector<std::string> params;
        req->get_command(command);
        req->get_params(params);
        cout << command << " ";
        for(auto & param : params) {
            cout << param << " ";
        }
        //3.响应信息
        auto resp = redisTask->get_resp();
        protocol::RedisValue result;
        resp->get_result(result);

        //遍历RedisValue的数据
        if(result.is_string()) {
            cout << "value is string, value:"
                    << result.string_value() << endl;
        } else if(result.is_array()) {
            cout << "value is array:" << endl;
            for(size_t i = 0; i < result.arr_size(); ++i) {
                cout << "arr[" << i << "]:" 
                        << result.arr_at(i).string_value() << endl;
            }
        }
    });
    //设置任务属性
    redisTask->get_req()->set_request("HSET",{"student",
            "name","Jackie","age","30"});

    //启动任务
    redisTask->start();
    waitGroup.wait();
    if(pnumber){
        delete pnumber;
        pnumber = nullptr;
    }
}   
int main()
{
    test();
    return 0;
}

