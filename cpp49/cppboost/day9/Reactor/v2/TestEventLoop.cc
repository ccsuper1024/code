/*  @file @brief @author chenchao @brief */
#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <unistd.h>
#include <iostream>


using std::endl;
using std::cout;
using std::cerr;
using std::cin;

//用于tcp三个半事件中三个事件的普通函数
//1.连接建立的事情
void onNewConnection(const TcpConnectionPtr& rhs) {
    fprintf(stdout,"%s has connected\n",rhs->toString().c_str());
}
//2.消息到达时的事情
void onMessgae(const TcpConnectionPtr& rhs) {
    std::string msg = rhs->receive();
    fprintf(stdout,">>recv msg from client %s\n",msg.c_str());

    //接到客户端的msg之后，在此处做业务逻辑处理，比如应用侧的解析，
    //数据的进一步处理等

    //数据发回给客户端
    rhs->send(msg);
}
//3.关闭连接的事情
void onClose(const TcpConnectionPtr& rhs) {
    fprintf(stdout,"%s has closed\n",rhs->toString().c_str());
}

/* @brief @pram @return */
void test(){
    Acceptor acceptor(8888);
    acceptor.ready();   //出于监听状态

    EventLoop loop(acceptor);
    loop.setNewConnectionCallback(std::move(onNewConnection));
    loop.setMessageCallback(std::move(onMessgae));
    loop.setCloseCallback(std::move(onClose));

    loop.loop();
}
int main()
{
    test();
    return 0;
}

