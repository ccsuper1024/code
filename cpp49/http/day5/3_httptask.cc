/*@file @brief*/
#include <signal.h>
#include <iostream>
#include <workflow/WFFacilities.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

static WFFacilities::WaitGroup waitgroup(1);
void sighandler(int signum)
{
    cout << "sig number" << signum << "is comming \n";
    waitgroup.done();
}
void httpCallback(WFHttpTask *httpTask)
{
    // 当回调函数被运行时，已经完成了数据的发送和接受了
    cout << "httpCallback is running " << endl;
    // 1.错误检测
    int state = httpTask->get_state();
    int error = httpTask->get_error();

    switch(state){
    case WFT_STATE_SYS_ERROR:
        cout << "system error: " << strerror(error) << endl;
         break;
    case WFT_STATE_DNS_ERROR:
        cout << "dns error: " << gai_strerror(error) << endl;
         break;
    case WFT_STATE_SUCCESS:
        break;
    }
    /*switch (state)
    {
    case WFT_STATE_SYS_ERROR:
        printf("system error: %s\n", strerror(error));
        break;
    case WFT_STATE_DNS_ERROR:
        printf("dns error: %s\n", gai_strerror(error));
        break;
    case WFT_STATE_SUCCESS:
        break;
    }*/
    if (state != WFT_STATE_SUCCESS)
    {
        cout << "error occurs " << endl;
        return;
    }
    cout << "task success! " << endl;

    // 2.获取请求报文的信息
    // 2.1起始行
    auto req = httpTask->get_req();
    cout << req->get_method() << req->get_request_uri()
         << req->get_http_version() << endl;

    // 2.2首部字段，需要通过一个迭代器进行遍历
    protocol::HttpHeaderCursor cursor(req);
    std::string key, value;
    while (cursor.next(key, value))
    {
        cout << key << ": " << value << endl;
    }
    cout << endl;

    // 3.获取响应报文的信息
    auto resp = httpTask->get_resp();
    // 3.1起始行
    cout << resp->get_http_version() << " ";  // http版本信息
    cout << resp->get_status_code() << " " ;   // 状态码
    cout << resp->get_reason_phrase()<<endl; // 原因短语

    // 3.2首部字段
    protocol::HttpHeaderCursor cursor2(resp);
    while (cursor2.next(key, value))
    {
        cout << key << ": " << value << endl;
    }

    // 3.3消息体
    const void *body = nullptr;
    size_t sz = 0;
    resp->get_parsed_body(&body, &sz);
    cout << *static_cast<const char *>(body) << endl;
}
/* @brief @pram @return */
void test()
{
    // 注册信号
    signal(SIGINT, sighandler);

    // 创建任务
    std::string url = "http://www.baidu.com";
    //原型：WFHttpTask* create_http_task(const string & url,int redirect_max,int retry_max,http_callback_t callback);
    //using http_callback_t = std::function<void(WFHttpTask *)>
    //第一个参数是url类型的地址，第二个参数为重定向次数，第三个参数是重试次数，第四个参数是回调函数
    WFHttpTask *httpTask = WFTaskFactory::create_http_task(url, 1, 1, 
        httpCallback);

    // 设置任务属性
    auto req = httpTask->get_req();
    req->add_header_pair("User-Agent", "workflow http client");

    // 立即启动任务。使用start()方法是立即异步启动线程的
    httpTask->start();
    // 阻塞等待
    waitgroup.wait();
}
int main()
{
    test();
    return 0;
}
