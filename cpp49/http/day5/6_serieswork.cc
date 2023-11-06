/*@file 6_serieswork.cc @brief 错误执行workflow串行任务的方式*/
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include <workflow/WFFacilities.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/RedisMessage.h>

using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::cerr;
using std::cin;
static WFFacilities::WaitGroup waitGroup(1);

void sighandler(int num)
{
    printf("sig number %d is comming\n", num);
    waitGroup.done();
}

void redisCallback(WFRedisTask * redistask)
{
    cout << "redisCallbakc is comming"<< endl;
    //错误检测
    int state = redistask->get_state();
    int error = redistask->get_error();

    switch(state){
        case WFT_STATE_SYS_ERROR:
            cout << "system error:" << strerror(error) << endl;
            break;
        case WFT_STATE_DNS_ERROR:
            cout << "DNS error: " << gai_strerror(error) <<endl;
            break;
        case WFT_STATE_SUCCESS:
            cout << "task success" << endl;
            break;
    }
    if(state != WFT_STATE_SUCCESS){
        cout << "error occurs" << endl;
        return;
    }
    //请求信息
    protocol::RedisRequest* req = redistask->get_req();
    string command;
    vector<string> params;
    req->get_command(command);
    req->get_params(params);
    cout << "params: " ;
    for(auto& e :params){
        cout << e << " "; 
    }
    cout << endl;
    //响应信息
    protocol::RedisResponse* resp = redistask->get_resp();
    protocol::RedisValue result;
    resp->get_result(result);
    //遍历数据
    if(result.is_string()){
        cout << "value is string ,value: " 
            << result.string_value() << endl;
    }else if(result.is_int()){
        cout << "value is integer,value:"
            << result.int_value() << endl;
    }else if(result.is_array()){
        cout << "value is array: "<< endl;
        for(size_t i = 0; i < result.arr_size(); i++){
            cout << "arr[" << i << "]:" 
                 << result.arr_at(i).string_value() << endl;
        }
    }

}

/* @brief @pram @return */
void test(){
     signal(SIGINT, sighandler);
    std::string url = "redis://127.0.0.1:6379";
    //创建Redis任务
    auto redisTask = WFTaskFactory::create_redis_task(url, 1, redisCallback);
    //设置任务的属性
    auto req = redisTask->get_req();
    req->set_request("hset", {"student", "name", "Jackie", "age", "30"});
    //交给框架来调度运行
    redisTask->start();

    //sleep(1); //休眠后第一个任务产生的序列已经执行完毕了，并且销毁了

    //注意：如果是按照当前的方式执行串行任务的话是有风险的。
    //即:先立即运行第一个redisTask再通过series of全局函数去把第二个redisTask2加入序列
    //会有可能导致第二个任务不会执行。因为workflow中的任务是非阻塞的。一旦第一个任务出现
    //阻塞,或者在第一个任务和第二个任务之间加上了sleep(1)这种代码，就会出现第二个任务
    //不执行的情况
    auto redisTask2 = WFTaskFactory::create_redis_task(url,1,redisCallback);
    redisTask2->get_req()->set_request("HGETALL",{"student"});

    //通过任务本身获取其所在的序列,串行执行，保证任务的执行顺序
    series_of(redisTask)->push_back(redisTask2);

    //如果是以下代码则是并行执行
    //redisTask->start();
    //redisTask2->start();

    waitGroup.wait();
}
int main()
{
    test();
    return 0;
}

