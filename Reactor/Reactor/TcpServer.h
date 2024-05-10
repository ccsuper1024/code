//
// Created by chenchao on 24-5-8.
//
#ifndef REACTOR_TCPSERVER_H
#define REACTOR_TCPSERVER_H
#include "Acceptor.h"
#include "EventLoop.h"
class TcpServer
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
public:
    TcpServer(const std::string& ip, unsigned short port);
    ~TcpServer();

    //让服务器运行起来
    void start();
    //停止服务器
    void stop();
    //对三个半事件注册回调函数
    void setAllCallback(TcpConnectionCallback&& onConnection,
                        TcpConnectionCallback&& onMessage,
                        TcpConnectionCallback&& onClose);
private:
    //组合
    Acceptor _acceptor;
    EventLoop _loop;
};


#endif //REACTOR_TCPSERVER_H
