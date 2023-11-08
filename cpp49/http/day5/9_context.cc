/*@file 9_context.cc @brief 任务间传递数据的方式 2*/
//通过序列任务中的上下文指针。序列中的各个任务可以通过这个指针去访问各个任务之间共享
//的数据
//缺点：由于这个指针是被protected的，需要通过成员函数去设置，同时这个共享数据是堆上的
//内存，且没有被智能指针或者RAII方法约束，所以需要程序员自己注册序列的回调函数来释放
//内存。
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
void sighandler(int signum){
    cout << "signum: " << signum << endl;
    waitGroup.done();
}
//序列任务本身也可以设定回调函数
void seriesCallback(const SeriesWork* series)
{
    cout << "seriesCallback is running " << endl;
    //回收序列的上下文信息
    int* pnumber = (int*) series->get_context();
    if(pnumber){
        delete pnumber;
        pnumber = nullptr;
    }
}
void redisCallback2(WFRedisTask* redisTask){
    cout << "\nredisCallback2 is running" << endl;
    //错误的检测
    int state = redisTask->get_state();
    int error = redisTask->get_error();

    switch(state){
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
    //获取序列中的上下文数据
    int* pnumber = static_cast<int*>(series_of(redisTask)->get_context());
    cout << "pnumber: " << pnumber << endl;
    cout << "*pnumber: " << *pnumber << endl; 
    //获取请求信息
    protocol::RedisRequest *req = redisTask->get_req();
    string command;
    vector<string> params;
    req->get_command(command);
    req->get_params(params);
    cout << command << " ";
    for(auto& e:params){
        cout << e << " ";
    }
    cout << endl;
    //获取响应信息并创建序列任务
    protocol::RedisResponse* resp = redisTask->get_resp();
    protocol::RedisValue result;
    resp->get_result(result);

    //遍历响应的返回值
    if(result.is_string()){
        cout << "value is string,value:"
             << result.string_value() << endl;
    }else if(result.is_array()){
        cout << "value is array:" << endl;
        for(size_t i = 0; i < result.arr_size(); ++i){
            cout << "arr[" << i << "]:"
                << result.arr_at(i).string_value() << endl;
        }
    }
    waitGroup.done();
}
void redisCallback1(WFRedisTask* redisTask){
    cout << "redisCallback1 is running" << endl;
    //错误的检测
    int state = redisTask->get_state();
    int error = redisTask->get_error();

    switch(state){
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
    //设置序列共享的上下文
    int* pnumber = new int(10); //堆上申请资源，需要程序员手动释放
    series_of(redisTask)->set_context(pnumber);
    series_of(redisTask)->set_callback(seriesCallback);
    cout << "pnumber: " << pnumber<< endl;
    cout << "*pnumber: " << *pnumber << endl;
    //获取请求信息
    protocol::RedisRequest *req = redisTask->get_req();
    string command;
    vector<string> params;
    req->get_command(command);
    req->get_params(params);
    cout << command << " ";
    for(auto& e:params){
        cout << e << " ";
    }
    cout << endl;
    //获取响应信息并创建序列任务
    protocol::RedisResponse* resp = redisTask->get_resp();
    protocol::RedisValue result;
    resp->get_result(result);

    //遍历响应的返回值
    if(result.is_string()){
        cout << "value is string,value:"
             << result.string_value() << endl;
    }else if(result.is_int()){
        cout << "value is integer,value: "
             << result.int_value() << endl;
        //添加第二个redisTask
        string redisUrl = "redis://127.0.0.1:6379";
        auto redisTask2 = WFTaskFactory::create_redis_task(redisUrl,1,redisCallback2);
        redisTask2->get_req()->set_request("HGETALL",{"student"});

        //将任务1中的共享数据传递给任务2
        redisTask2->user_data = redisTask->user_data;
        //通过任务本身获取其所在的序列
        series_of(redisTask)->push_back(redisTask2);
    }else if(result.is_array()){
        cout << "value is array:" << endl;
        for(size_t i = 0; i < result.arr_size(); ++i){
            cout << "arr[" << i << "]:"
                << result.arr_at(i).string_value() << endl;
        }
    }
}


/* @brief @pram @return */
void test(){
    //注册信号
    signal(SIGINT,sighandler);
    //创建任务
    string redisUrl = "redis://127.0.0.1:6379";
    WFRedisTask* redisTask = WFTaskFactory::create_redis_task(redisUrl,1,redisCallback1);
    //设置任务属性
    protocol::RedisRequest* req = redisTask->get_req();
    req->set_request("HSET",{"student","name","Jackie","age","30"});
    //启动任务
    redisTask->start();
    waitGroup.wait();   //阻塞等待
}
int main()
{
    test();
    return 0;
}

