/*@file  httpserver.cc @brief workflow作为http的服务器。其是被动调用的。程序员要自己注册函数
 * 阶段：1.建立连接 2.解析请求 3. 生成响应 4. 发送消息.其中第1,4阶段不需要程序员来处理，只需要写
 * 第2,3阶段
 * 服务器需要提供一个process函数，在其中可以进行第二，三阶段的任务。在其中可以发起其他任务。
 * 比如redis任务，mysql任务等*/
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include <workflow/WFFacilities.h>
#include <workflow/WFHttpServer.h>
#include <workflow/HttpUtil.h>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

static WFFacilities::WaitGroup waitGroup(1);

void sighandler(int signum){
    cout << "sig number " << signum << " is comming\n" << endl;
    waitGroup.done();
}

void httpCallback(WFHttpTask* serverTask){
    cout << "httpCallback is running " << endl;
    cout << "serverTask addr: " << serverTask << endl;
}
void process(WFHttpTask* serverTask){
    cout << "\nprocess is running " << endl;
    cout << "serverTask addr: " << serverTask << endl;

    //设置serverTask的回调函数，在执行完process的基本动作后才会调用回调函数
    //using http_callback_t = std::function<void (WFHttpTask *)> 
    serverTask->set_callback(httpCallback);

    //解析请求
    auto req = serverTask->get_req();
    printf("%s %s %s\n",req->get_method(),
            req->get_request_uri(),req->get_http_version());
    protocol::HttpHeaderCursor cursor(req);
    std::string key,value;
    //bool next(std::string &name, std::string &value)
    while(cursor.next(key,value)){
        cout << key << ": " << value <<endl;
    }

    //2.生成响应
    auto resp = serverTask->get_resp();
    //2.构造起始行
    resp->set_http_version("HTTP/1.1");
    resp->set_status_code("201");
    resp->set_reason_phrase("OK");

    //2.2首部字段
    resp->add_header_pair("Content-Type","text/html");
    resp->add_header_pair("Server","Workflow HttpServer");
    //2.3消息实体
    std::string msg("<html><body>Welcome to workflow server</body></html>");
    //bool protocol::HttpMessage::append_output_body(const void *buf, size_t size)
    resp->append_output_body(msg.c_str(),msg.size());
    resp->add_header_pair("Content-Length",std::to_string(msg.size()).c_str());
}
/* @brief @pram @return */
void test(){
    signal(SIGINT,sighandler);
    //WFServer(http_process_t proc)
    //using WFHttpServer = WFServer<protocol::HttpRequest, protocol::HttpResponse>
    WFHttpServer server(process);
    if(server.start(8888) == 0){
        waitGroup.wait();
        server.stop();      //停止服务器
    }else{
        cout << "server start failed " << endl;
    }
}
int main()
{
    test();
    return 0;
}

