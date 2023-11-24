/*@file loginserver.cc @brief 使用workflow实现一个具有登录功能的服务器程序 */
/*如何实现一个登录功能？
1.注册redis任务/mysql任务
2.解析请求
3.在redis中验证。
由于server任务和redis任务是同一序列中的不同任务，所以要共享数据。这个数据是用户名，密码
和响应报文
*/
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <workflow/WFFacilities.h>
#include <workflow/WFHttpServer.h>
#include <workflow/HttpUtil.h>
#include <workflow/RedisMessage.h>
//#include <workflow/MySQLMessage.h>
using std::shared_ptr;
using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::cerr;
using std::cin;

static WFFacilities::WaitGroup waitGroup(1);
//共享数据
struct SeriesContext{
    string _password;
    protocol::HttpResponse* _resp;
    SeriesContext():_password(),_resp(nullptr)
    {}
    /* ~SeriesContext(){ */
    /*     if(_resp){ */
    /*         delete _resp; */
    /*         _resp = nullptr; */
    /*     } */
    /* } */
};
void sighandler(int signum){
    printf("sig number %d is comming\n", signum);
    waitGroup.done();
}
void httpCallback(WFHttpTask* serverTask){
    cout << "httpCallback is running" << endl;
    //serverTask的回调函数会在发出响应信息之后才会调用，所以对于响应信息的修改都要
    //在httpCallback之前的函数中修改。比如redisCallback中
    
}
void redisCallback(WFRedisTask* redisTask){
    cout << "redisCallback is running" << endl;
    //进行错误的检测
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

    //生成响应
    SeriesContext* context = (SeriesContext*)series_of(redisTask)->get_context();

    protocol::RedisValue result;
    redisTask->get_resp()->get_result(result);
    if(result.is_string()){
        cout << "value is string: " << result.string_value() << endl;
        if(result.string_value() == context->_password){
            //登录验证成功
            context->_resp->append_output_body("Login Success!");
            cout << "get password: " << context->_password << endl;
        }else{
            //登录验证失败
            context->_resp->append_output_body("Login Failed");
        }
    }else{
        //登录验证失败
        context->_resp->append_output_body("Login Failed2");
    }
    context->_resp->add_header_pair("Server","Workflow http server");
    context->_resp->add_header_pair("Content-Type","text/plain");
    context->_resp->set_http_version("HTTP/1.1");
    context->_resp->set_status_code("200");
    context->_resp->set_reason_phrase("Ok");

}
void serverSeriesCallback(const SeriesWork* serverSeriesWork){
    cout << "seriesCallback is running" << endl;
    SeriesContext* context = (SeriesContext*)serverSeriesWork->get_context();
    if(context){
        delete context;
        context = nullptr;
    }
}
void process(WFHttpTask* serverTask){
    cout << "process is running" << endl;
    //设置serverTask的回调函数
    serverTask->set_callback(httpCallback);
    //设置序列的回调函数
    //using series_callback_t = std::function<void (const SeriesWork *)> 
    series_of(serverTask)->set_callback(serverSeriesCallback);

    //url = http://192.168.30.128:8888/login?name=liubei&password=123
    //url = 协议字段://用户信息(用户名:密码)@ip(或域名):端口号/(路径名)？查询词1&查询词2#片段
    //1.解析请求
    auto req = serverTask->get_req();
    string uri = req->get_request_uri();
    cout << "uri: " << uri << endl;
    cout << "method: " << req->get_method() << endl;
    string path = uri.substr(0,uri.find("?"));
    string nameKV = uri.substr(0,uri.find("&"));
    string passwdKV = uri.substr(uri.find("&")+1);
    string name = nameKV.substr(nameKV.find("=") + 1);
    string passwd = passwdKV.substr(passwdKV.find("=") + 1);
    //如果是中文，还要考虑转换
    cout << "path: " << path << endl;
    cout << "name: " << name << endl;
    cout << "passwd: " << passwd << endl;

    //设置共享数据
    //shared_ptr<SeriesContext> context(new SeriesContext());
    SeriesContext* context = new SeriesContext();
    context->_password = passwd;
    context->_resp = serverTask->get_resp();
    //static inline SeriesWork *series_of(const SubTask *task)
    //inline void SeriesWork::set_context(void *context)
    series_of(serverTask)->set_context(context);

    //进行登录验证
    if(path == "/login" && req->get_method() == string("GET")){
        //创建Redis任务
        cout << "111" << endl;
        auto redisTask = WFTaskFactory::create_redis_task("redis://127.0.0.1:6379",
                            1,redisCallback);
        //设置redis任务属性
        string command("HGET");
        vector<string> params{"student",name};
        redisTask->get_req()->set_request(command,params);
        //将redis任务加入当前servertask任务的序列之后
        series_of(serverTask)->push_back(redisTask);
    }else{
        cout << "222" << endl;
    }
}
/* @brief @pram @return */
void test(){
    signal(SIGINT,sighandler);

    WFHttpServer server(process);

    if(server.start(8888) == 0){
        waitGroup.wait();
        server.stop();  //停止服务器
    }else{
        cout << "server start failed" << endl;
    }
}
int main()
{
    test();
    return 0;
}

