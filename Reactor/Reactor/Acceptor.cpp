//
// Created by chenchao on 24-5-8.
//
#include "Acceptor.h"

Acceptor::Acceptor(const std::string& ip, unsigned short port)
:_sock()
 ,_addr(ip, port)
 ,_peerAddr()
{
}
void Acceptor::setReuseAddr() { //设置地址复用
    int on = 1;
    int ret = setsockopt(_sock.fd(),SOL_SOCKET,
                             SO_REUSEADDR,&on, sizeof(on));
    if(-1 == ret) {
        fprintf(stderr, "setsockopt error");
        //TODO:LOG
        return ;
    }
}
void Acceptor::setReusePort() {   //设置端口复用
    int on = 1;
    int ret = setsockopt(_sock.fd(), SOL_SOCKET,
                         SO_REUSEPORT, &on, sizeof(on));
    if(-1 == ret) {
        fprintf(stderr, "setsockopt error");
        //TODO:LOG
        return ;
    }
}
void Acceptor::bind() {
    int ret = ::bind(_sock.fd(), (struct sockaddr*)_addr.getInetAddrPtr(),
                     sizeof(struct sockaddr));
    if(-1 == ret) {
        fprintf(stderr, "bind error");
        //TODO:LOG
        return ;
    }

}
void Acceptor::listen() {   //监听。
    //socket系统调用创建的内核文件的输入输出缓冲区被改为半连接队列和全连接队列。
    //而这两个队列的大小是listen()系统调用的第二个参数
    int ret = ::listen(_sock.fd(), 128);
    if(-1 == ret) {
        fprintf(stderr, "listen error");
        //TODO:LOG
        return ;
    }

}
int Acceptor::fd() {
    return _sock.fd();
}
void Acceptor::ready() {
    this->setReuseAddr();
    this->setReusePort();
    this->bind();
    this->listen();
}

//很明显 只要使用IO多路复用，那么网络编程中的accept()系统调用和其他诸如socket(),bind(),listen()的
//函数要分开
int Acceptor::accept() {
    socklen_t len = sizeof((*_peerAddr.getInetAddrPtr()));
    int connfd = ::accept(_sock.fd(), (struct sockaddr*)_peerAddr.getInetAddrPtr(),
                          &len);
    if(-1 == connfd) {
        fprintf(stderr, "accept error");
        //TODO:LOG
        return -1;
    }
    return connfd;
}

InetAddress Acceptor::getPeerInetAddress() {
    return _peerAddr;
}


