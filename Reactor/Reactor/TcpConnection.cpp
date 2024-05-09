//
// Created by chenchao on 24-5-8.
//

#include "TcpConnection.h"
TcpConnection::TcpConnection(int fd,EventLoop* loop)
:
{}
void TcpConnection::send(const std::string& msg){}
void TcpConnection::sendLoop(const std::string& msg) {  //让msg在EventLoop中发送

}
std::string TcpConnection::receive(){}
std::string TcpConnection::toString(){}
InetAddress TcpConnection::getLocalAddr(){}
InetAddress TcpConnection::getPeerAddr(){}
bool TcpConnection::isClosed(){}
void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback& cb){}
void TcpConnection::setMessageCallback(const TcpConnectionCallback& cb){}
void TcpConnection::setCloseCallback(const TcpConnectionCallback& cb){}
void TcpConnection::handleNewConnectionCallback(){}
void TcpConnection::handleMessageCallback(){}
void TcpConnection::handleCloseCallback(){}
