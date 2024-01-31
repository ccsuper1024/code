#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;

class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
public:
    explicit TcpConnection(int fd);
    ~TcpConnection();
    void send(const string &msg);
    string receive();
    //为了方便调试的函数
    string toString();
    //判断连接是否断开了
    bool isClosed() const;
    
    //在TcpConnection中需要注册三个回调函数
    void setNewConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);

    //执行对应的三个回调函数
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    //获取本端地址与对端地址
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    SocketIO _sockIO;

    //为了调试而加入的函数
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnectionCb;//1、连接建立
    TcpConnectionCallback _onMessageCb;//2、消息到达
    TcpConnectionCallback _onCloseCb;//3、连接断开
};

#endif
