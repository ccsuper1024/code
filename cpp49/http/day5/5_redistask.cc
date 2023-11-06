/*@file 5_redistask.cc @brief workflow中的串行任务*/

#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include <workflow/WFTaskFactory.h>
#include <workflow/WFFacilities.h>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
static WFFacilities::WaitGroup waitGroup(1);

void sighandler(int signum)
{
    cout << "sig number " << signum << "is comming" << endl;
    waitGroup.done();
}
void redisCallback(WFRedisTask *redisTask)
{
    cout << "redisCallback is running " << endl;
    // 错误检测
    int state = redisTask->get_state();
    int error = redisTask->get_error();

    switch (state)
    {
    case WFT_STATE_SYS_ERROR:
        cerr << "system error: " << strerror(error) << endl;
        break;
    case WFT_STATE_DNS_ERROR:
        cerr << "dns error: " << gai_strerror(error) << endl;
        break;
    case WFT_STATE_SUCCESS:
        cout << "task success" << endl;
        break;
    }
    if (state != WFT_STATE_SUCCESS)
    {
        cerr << "error occurs" << endl;
        return;
    }
    // 获取请求信息
    auto req = redisTask->get_req();
    string command;
    vector<string> params;
    req->get_command(command);
    req->get_params(params);

    cout << "params: ";
    for (auto &e : params)
    {
        cout << e << " ";
    }
    cout << endl;
    // 获取响应信息
    auto resp = redisTask->get_resp();
    protocol::RedisValue result;
    resp->get_result(result);
    // 遍历RedisValue的数据
    if (result.is_string())
    {
        cout << "value is string, value:" << result.string_value() << endl;
    }
    else if (result.is_int())
    {
        cout << "value is integer, value:" << result.int_value() << endl;
    }
    else if (result.is_array())
    {
        for (size_t i = 0; i < result.arr_size(); ++i)
        {
            cout << "arr[" << i << "]: " << result.arr_at(i).string_value()
                 << endl;
        }
    }
}
/* @brief @pram @return */
void test()
{
    // 注册信号
    signal(SIGINT, sighandler);
    // 创建redis任务
    string redisUrl = "redis://127.0.0.1:6379";
    //原型：WFHttpTask* create_redis_task(const string & url,int retry_max,redis_callback_t callback);
    //using redis_callback_t = std::function<void(WFRedisTask *)>
    WFRedisTask *redisTask = WFTaskFactory::create_redis_task(redisUrl, 
                                1, redisCallback);
    // 设置任务属性
    auto req = redisTask->get_req();
    req->set_request("hset", {"student", "name", "Jackie", "age", "30"});
    // req->set_request("HGETALL",{"student"})
    // 开启框架
    redisTask->start();
    // 阻塞等待
    waitGroup.wait();
}
int main()
{
    test();
    return 0;
}
