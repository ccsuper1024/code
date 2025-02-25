/*  @file @author chenchao @brief */

//c std header
#include <stdio.h>
#include <signal.h>
//c++ std header
//#include <iostream>

//c third header

//c++ third header

//c custom header

//c++ custom header
#include "example.srpc.h"

using std::endl;
using std::cout;
using std::cerr;
using std::cin;
using namespace srpc;
class ExampleServiceImpl
    :public Example::Service
{
public:
    void Echo(EchoRequest* request, EchoResponse* response, RPCContext* ctx) override
    {
        response->set_message("Hi " + request->name());
        printf("get_req:\n%s\nset_resp\n%s\n",
               request->DebugString().c_str(), response->DebugString().c_str());
    }
};

void sig_handler(int signo) {

}

/* @brief @pram @return */
void test(){
    // signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    SRPCServer server_tcp;
    SRPCHttpServer server_http;

    ExampleServiceImpl impl;
    server_tcp.add_service(&impl);
    server_http.add_service(&impl);

    server_tcp.start(1412);
    server_http.start(881);
    getchar();              //press Enter to end
    server_http.stop();
    server_tcp.stop();

}
int main()
{
    test();
    return 0;
}
