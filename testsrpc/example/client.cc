/*  @file @author chenchao @brief */

//c std header
#include <stdio.h>
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

/* @brief @pram @return */
void test(){
    //构造对象
    Example::SRPCClient client("127.0.0.1", 1412);
    EchoRequest req;
    
    //赋值
    req.set_message("Hello, srpc!\n");
    req.set_name("workflow\n");

    client.Echo(&req, [](EchoResponse* response, RPCContext* ctx){
        if(ctx->success()) {
            printf("%s\n", response->DebugString().c_str());
        } else {
            printf("status[%d] error[%d] errmsg:%s\n",
                    ctx->get_status_code(), ctx->get_error(), ctx->get_errmsg());
        }
    });

    getchar();      //press "Enter" to end
}
int main()
{
    test();
    return 0;
}
        
