//
// Created by chenchao on 24-5-8.
//
#include "EventLoop.h"
#include "TcpConnection.h"
#include <sys/socket.h>
#include <sstream>
TcpConnection::TcpConnection(int fd,EventLoop* loop)
:_sock(fd)
, _sockIO(fd)
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
,_loop(loop)
{

}
void TcpConnection::send(const std::string& msg) {
    _sockIO.writen(msg.c_str(), msg.size());
}

//将msg发送给EventLoop，然后让EventLoop将数据发送给客户端
//因为EventLoop本身没有发送数据的能力，所以再次出需要将msg
//以及具有发送msg能力的TcpConnection::send一起传递给EventLoop.
//为什么要这样子设计？？TcpConnection类到底是干什么的？？
void TcpConnection::sendLoop(const std::string& msg) {  //让msg在EventLoop中发送
    if(_loop) {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}
std::string TcpConnection::receive() {
    char buff[65535] = {0};     //64kb
    _sockIO.readLine(buff, sizeof(buff));

    return std::string(buff);       //构造临时对象，传递给返回值时，发生复制
}
std::string TcpConnection::outputString() {
   std::ostringstream oss;
   oss << _localAddr.ip() << ":"
        << _localAddr.port() << ":"
        << _peerAddr.ip()<< ":"
        << _peerAddr.port()<<std::endl;
   return oss.str();
}
InetAddress TcpConnection::getLocalAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getsockname(_sock.fd(), (struct sockaddr*)&addr, &len);
    if (-1 == ret) {
        fprintf(stderr, "getlocalname error\n");
        //TODO:log
        return InetAddress();
    }
    return InetAddress(addr);
}
InetAddress TcpConnection::getPeerAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = getpeername(_sock.fd(), (struct sockaddr*)&addr, &len);
    if (-1 == ret) {
        fprintf(stderr, "getpeername error\n");
        //TODO:log
        return InetAddress();
    }
    return InetAddress(addr);

}

//检测发送端连接断开的方法是使用recv函数。
//因为recv的返回值是返回数据的字节长度;或者若无可用数据或对等方已经按序结束，则会返回0，若出错则返回-1；
//但这只能通过返回值为0检测到发送方主动关闭连接，无法判定因为网络异常等客观原因而出现的连接断开问题，这些问题
//可以通过心跳协议来进一步判断。
bool TcpConnection::isClosed() {
    char buf[10] = {0};
    //第四个参数表明，recv只是拷贝内核文件中输入缓冲区数据，不会改变其中的指针。下一次使用正常的阻塞的read
    //还是会从缓冲区的头部开始读
    int ret = recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);
    return(0 == ret);
}
void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback& cb) {
    _onConnectionCb = cb;
}
void TcpConnection::setMessageCallback(const TcpConnectionCallback& cb) {
    _onMessageCb = cb;
}
void TcpConnection::setCloseCallback(const TcpConnectionCallback& cb) {
    _onCloseCb = cb;
}
void TcpConnection::handleNewConnectionCallback() {
    if(_onConnectionCb) {
        //using TcpConnectionCallback = std::function<void(const TcpConnectionPtr& )>;
        //TcpConnectionCallback的变量_onConnectionCb是一个函数对象，参数为const TcpConnectionPtr&
        //TcpConnectionPtr是TcpConnection的智能指针,所以TcpConnection类要继承std::enable_shared_from_this<>
        //返回值为void
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
