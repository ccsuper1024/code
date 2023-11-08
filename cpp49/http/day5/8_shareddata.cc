/*@file 8_shareddata.cc @brief workflow的任务之间的共享数据传递 1*/
//redistTask的类类型是如何继承而来的
//using WFRedisTask = WFNetworkTask<protocol::RedisRequest,protocol::RedisResponse>;
//template<class REQ, class RESP>
//class WFNetworkTask : public CommRequest
//class CommRequest : public SubTask, public CommSession
/*class SubTask{        //抽象基类
    ... 
    private:
	ParallelTask *parent;
	SubTask **entry;
	void *pointer;
...
}*/
/*class CommSession     //抽象基类
{
private:
	virtual CommMessageOut *message_out() = 0;
	virtual CommMessageIn *message_in() = 0;
	virtual int send_timeout() { return -1; }
	virtual int receive_timeout() { return -1; }
	virtual int keep_alive_timeout() { return 0; }
	virtual int first_timeout() { return 0; }	//for client session only. 
	virtual void handle(int state, int error) = 0;

protected:
	CommTarget *get_target() const { return this->target; }
	CommConnection *get_connection() const { return this->conn; }
	CommMessageOut *get_message_out() const { return this->out; }
	CommMessageIn *get_message_in() const { return this->in; }
	long long get_seq() const { return this->seq; }

private:
	CommTarget *target;
	CommConnection *conn;
	CommMessageOut *out;
	CommMessageIn *in;
	long long seq;

private:
	struct timespec begin_time;
	int timeout;
	int passive;

public:
	CommSession() { this->passive = 0; }
	virtual ~CommSession();
	friend class CommMessageIn;
	friend class Communicator;
};*/

#include <signal.h> //注册信号
#include <iostream>
#include <string>
#include <vector>
#include <workflow/WFFacilities.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/RedisMessage.h>
#include <workflow/HttpMessage.h>

using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::cerr;
using std::cin;

//waitgroup
static WFFacilities::WaitGroup waitGroup(1);
void sighandler(int signum){
    printf("sig number %d is comming\n", signum);
    waitGroup.done();
}
void httpCallback(WFHttpTask* httpTask){

}
void redisCallback(WFRedisTask* redisTask){
    //错误检测
    cout << "redisCallback is running" << endl;
    //1. 错误的检测
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
    //显示共享数据
    int* pnumber = (int*)redisTask->user_data;
    cout << "pnumber: " << pnumber << endl;
    cout << "*pnumber: " << *pnumber << endl;
    cout << "redisCallback is running " << endl;
    //获取响应信息，判断是否添加串行任务到当前序列中
    protocol::RedisRequest* req = redisTask->get_req();
    string command;
    vector<string > params;
    req->get_command(command);
    req->get_params(params);
    cout << command << " ";
    for(auto & param : params) {
        cout << param << " ";
    }
    cout << endl;

    //获取响应信息
    protocol::RedisResponse* resp = redisTask->get_resp();
    protocol::RedisValue result;
    resp->get_result(result);

    //遍历RedisValue的数据
    if(result.is_string()){
        cout << "value is string, value:"
             << result.string_value() << endl;
    }else if(result.is_int()){
        cout << "value is integer, value:"
             << result.int_value() << endl;
        //添加第二个redisTask
        string redisUrl = "redis://localhost:6379";
        WFRedisTask* redisTask2 = WFTaskFactory::create_redis_task(redisUrl,1,redisCallback);
        //设置任务属性
        redisTask2->get_req()->set_request("HGETALL",{"student"});
        //获取任务间的共享数据
        redisTask2->user_data = redisTask->user_data;
        //通过任务本身获取其所在的序列
        //函数原型：static inline SeriesWork *series_of(const SubTask *task)
        series_of(redisTask)->push_back(redisTask2);
    }else if(result.is_array()) {
        cout << "value is array:" << endl;
        for(size_t i = 0; i < result.arr_size(); ++i) {
            cout << "arr[" << i << "]:" 
                 << result.arr_at(i).string_value() << endl;
        }
    }
}

/* @brief @pram @return */
void test(){
    //注册信号
    signal(SIGINT, sighandler);
    //创建任务。如果是通过ssl访问的reids。协议字段为rediss（多了个s）.格式为"rediss://password@host:port/dbnum?query#fragment"
    string redisUrl = "redis://localhost:6379";
    WFRedisTask* redisTask = WFTaskFactory::create_redis_task(redisUrl,
                                1,redisCallback);
    //共享数据
    int number = 1;
    //将test()栈空间里的数据保存到任务1中
    redisTask->user_data = (void *)&number;
    //设置任务的属性
    redisTask->get_req()->set_request("HSET",{"student",
                    "name ","Jackie","age","30"});

    //立即启动任务，并在结束后销毁其series任务
    redisTask->start();
    waitGroup.wait();    
}
int main()
{
    test();
    return 0;
}

