/*@file 11_parallelwork.cc @brief 构建三个http任务的并行任务*/
// workflow的基本单位是SeriesWork。即使是ParallelWork也要在SeriesWork内运行才可以
// 所以这三个http任务要先包含在序列任务中，再被加入到并行任务中，最后这个并行任务要加
// 入一个新的序列任务中
// 这三个http任务要与nginx代理服务器程序连接
#include <signal.h>
#include <iostream>
#include <workflow/WFFacilities.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

static WFFacilities::WaitGroup waitGroup(1);

void sighandler(int num)
{
    printf("sig number %d is comming\n", num);
    waitGroup.done();
}
void parallelSeriesCallback(const SeriesWork *parallelWork)
{
    cout << "\n parallelSeriesCallback is running" << endl;
    waitGroup.done(); // 结束框架
}
void parallelCallback(const ParallelWork *parallelWork)
{
    cout << "\n parallelCallback is running" << endl;
}
void httpCallback(WFHttpTask *httpTask);
/* @brief @pram @return */
void test()
{
    signal(SIGINT, sighandler);
    // using parallel_callback_t = std::function<void (const ParallelWork *)>
    // ParallelWork * create_parallel_work(parallel_callback_t callback)
    ParallelWork *parallenWork = Workflow::create_parallel_work(parallelCallback);

    std::vector<std::string> webs = {// 初始化列表，默认调用复制构造函数
                                     "http://localhost",
                                     "http://localhost:8081",
                                     "http://localhost:8082"};

    for (size_t i = 0; i < webs.size(); ++i)
    {
        // 创建任务
        auto httpTask = WFTaskFactory::create_http_task(webs[i],
                                                        1, 1, httpCallback);
        auto req = httpTask->get_req();
        req->add_header_pair("User-Agent", "Workflow http client");
        req->add_header_pair("Accept", "*/*");
        // 创建一个序列
        auto series = Workflow::create_series_work(httpTask, nullptr);
        // 将序列放入到并行任务
        parallenWork->add_series(series);
    }
    // 将并行任务放入一个序列中，开始运行
    Workflow::start_series_work(parallenWork, parallelSeriesCallback);
}
int main()
{
    test();
    return 0;
}

void httpCallback(WFHttpTask *httpTask)
{
    std::cout << "httpCallback1 is running " << endl;

    // 1.错误的检测
    int state = httpTask->get_state();
    int error = httpTask->get_error();

    switch (state)
    {
    case WFT_STATE_SYS_ERROR:
        printf("system error: %s\n", strerror(error));
        break;
    case WFT_STATE_DNS_ERROR:
        printf("dns error: %s\n", gai_strerror(error));
        break;
    case WFT_STATE_SUCCESS:
        break;
    }

    if (state != WFT_STATE_SUCCESS)
    {
        printf("error occurs!\n");
        return;
    }

    printf("task success\n");
    // 获取http的请求报文信息
    // 2.1起始行
    protocol::HttpRequest *req = httpTask->get_req();
    cout << req->get_method() << " " << req->get_http_version()
         << " " << req->get_request_uri() << endl;

    // 2.2首部字段，需要通过一个迭代器进行遍历
    protocol::HttpHeaderCursor cursor(req);
    std::string key, value;
    while (cursor.next(key, value)){
        cout << key << ": " << value << endl;
    }
    cout << endl;

    //3.获得响应报文的信息
    auto resp = httpTask->get_resp();
    //3.1起始行
    cout << resp->get_http_version() << " " << resp->get_status_code() << " "
        << resp->get_reason_phrase() << endl; 
    //3.2首部字段
    protocol::HttpHeaderCursor cursor2(resp);
    while(cursor2.next(key,value)){
        cout << key << ": " << value << endl;
    }  
    //3.3消息体
    const void* body = nullptr;
    size_t sz = 0;
    //inline bool protocol::HttpMessage::get_parsed_body(const void **body, size_t *size) const
    //第一个参数为二级指针
    resp->get_parsed_body(&body,&sz);
    cout << (const char*)body;
}