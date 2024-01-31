/* @author chenchao @date @file @brief*/
#ifndef  _TCPCONNECTION_HPP
#define  _TCPCONNECTION_HPP
#include <functional>
#include <memory>
#include "InetAddress.h"
//前向声明
class SocketIO;
class TcpConnection
:public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
public:
/* @file @brief @pram*/
    explicit TcpConnection(int fd);
    void send(const std::string& msg);
    std::string receive();
    ~TcpConnection();

    //设置回调函数
    void setNewConnectionCallback(const TcpConnectionCallback& cb);
    void setMessageCallback(const TcpConnectionCallback& cb);
    void setCloseCallback(const TcpConnectionCallback& cb);

    //便于调试
    std::string toString();
    //判断连接是否断开
    bool isClosed()const;
    //执行三个回调函数
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
private:
    //获取本端地址与对端地址
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    std::unique_ptr<SocketIO> _sockio;  //每个TcpConnection都独占一个连接文件描述符
    InetAddress _localAddr;
    InetAddress _peerAddr;
    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};
#endif      //_TCPCONNECTION_HPP

