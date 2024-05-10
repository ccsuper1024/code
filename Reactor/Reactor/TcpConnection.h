//
// Created by chenchao on 24-5-8.
//
#ifndef REACTOR_TCPCONNECTION_H
#define REACTOR_TCPCONNECTION_H
#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include <memory>
#include <functional>

class EventLoop;        //前向声明
class TcpConnection
:public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr& )>;
public:
    explicit  TcpConnection(int fd, EventLoop* loop);
    void send(const std::string& msg);
    void sendLoop(const std::string& msg);  //让msg在EventLoop中发送
    std::string receive();
    std::string outputString();
    bool isClosed();
    void setNewConnectionCallback(const TcpConnectionCallback& cb);
    void setMessageCallback(const TcpConnectionCallback& cb);
    void setCloseCallback(const TcpConnectionCallback& cb);
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

    ~TcpConnection()=default;
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    //前向声明是不完整类型，可以使用不完整类型的指针或引用。因为指针或引用的大小是确定的
    EventLoop* _loop;                           //让TcpConnection知道EventLoop的存在
    TcpConnectionCallback _onConnectionCb;      //1.连接建立
    TcpConnectionCallback _onMessageCb;         //2.消息到达
    TcpConnectionCallback _onCloseCb;           //3.连接断开
};


#endif //REACTOR_TCPCONNECTION_H
