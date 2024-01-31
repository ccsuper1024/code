/* @author chenchao @date @file @brief*/
#include "TcpConnection.h"

#include "SocketIO.h"
#include <vector>
#include <iostream>
#include <sstream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
TcpConnection::TcpConnection(int fd)
:_sockio(std::make_unique<SocketIO>(fd))
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
{
    fprintf(stdout,"TcpConnection(int fd)\n");
}
void TcpConnection::send(const std::string& msg) {
    _sockio->writen(msg.c_str(),msg.size());
}
std::string TcpConnection::receive() {
    std::vector<char> vech;
    vech.reserve(65535);    //预留65535个char类的空间
    _sockio->readn(vech.data(),vech.size());

    return std::string(vech.begin(),vech.end());
}
TcpConnection::~TcpConnection() {
    close(_sockio->getFd());
    fprintf(stdout,"~TcpConnection()\n");
}

//设置回调函数
void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback& cb) {
    _onConnectionCb = cb;
}
void TcpConnection::setMessageCallback(const TcpConnectionCallback& cb) {
    _onMessageCb = cb;
}
void TcpConnection::setCloseCallback(const TcpConnectionCallback& cb) {
    _onCloseCb = cb;
}

//便于调试
std::string TcpConnection::toString() {
    std::ostringstream oss; //字符串IO
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();   //输出底层字符串设备对象的内容
}
//判断连接是否断开
bool TcpConnection::isClosed()const {
    char buf[10] = {0};
    int ret = recv(_sockio->getFd(),buf,sizeof(buf),MSG_PEEK);

    return (0==ret);
}
//执行三个回调函数
void TcpConnection::handleNewConnectionCallback() {
    if(_onConnectionCb) {
        //为什么此处回调函数的调用中要传入自己的指针??
        _onConnectionCb(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback() {
    if(_onMessageCb) {
        _onMessageCb(shared_from_this());
    }
}
void TcpConnection::handleCloseCallback() {
    if(_onCloseCb) {
        _onCloseCb(shared_from_this());
    }
}
//获取本端地址与对端地址
InetAddress TcpConnection::getLocalAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    //获取本端地址的函数getsockname
    int ret = ::getsockname(_sockio->getFd(),(struct sockaddr*)&addr,&len);
    if(-1 == ret) {
        perror("getsockname");
    
    }
    return InetAddress(addr);
}
InetAddress TcpConnection::getPeerAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    //获取本端地址的函数getsockname
    int ret = ::getpeername(_sockio->getFd(),(struct sockaddr*)&addr,&len);
    if(-1 == ret) {
        perror("getpeername");
    
    }
    return InetAddress(addr);
}