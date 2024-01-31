/* @author chenchao @date @file @brief*/
#include "Acceptor.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
Acceptor::Acceptor(unsigned short port,const std::string & ip,int backlog) 
:_sock()
,_addr(port,ip)
,_backlog(backlog)
{
    fprintf(stdout,"Acceptor(unsigned short& port"
                ",const std::string & ip,int backlog)\n");
}
Acceptor::~Acceptor() {
    //析构函数会自动先析构类成员对象再析构当前对象
}
void Acceptor::ready() {
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}
int Acceptor::accept() {
    //第二，三个参数是同一连接上对端的地址
    int connfd = ::accept(_sock.getFd(),nullptr,nullptr);
    if(connfd  == -1) {
        perror("accept\n");
        return -1;
    }
    return connfd;
}
void Acceptor::setReuseAddr() {
    int optval = 1;
    int ret = setsockopt(_sock.getFd(),SOL_SOCKET,SO_REUSEADDR,
                            &optval,sizeof(optval));
    if(-1 == ret) {
        perror("setReuseAddr\n");
        exit(EXIT_FAILURE);
    }
}
void Acceptor::setReusePort() {
    int optval = 1;
    int ret = setsockopt(_sock.getFd(),SOL_SOCKET,SO_REUSEPORT
                            ,&optval,sizeof(optval));
    if(-1 == ret) {
        perror("setReusePort\n");
        exit(EXIT_FAILURE);
    }                      

}
void Acceptor::bind() {
    int ret = ::bind(_sock.getFd(),(struct sockaddr*)_addr.getInetAddr(),
                        sizeof(struct sockaddr));
    if(-1 == ret ) {
        perror("bind\n");
        exit(EXIT_FAILURE);
    }
}
void Acceptor::listen() {
    int ret = ::listen(_sock.getFd(),_backlog);
    if(-1 == ret ) {
        perror("listen\n");
        exit(EXIT_FAILURE);
    }
}
int Acceptor::getFd() {
    return _sock.getFd();
}