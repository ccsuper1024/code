//
// Created by chenchao on 24-5-8.
//

#ifndef REACTOR_ACCEPTOR_H
#define REACTOR_ACCEPTOR_H
#include <string>
#include "InetAddress.h"
#include "Socket.h"

class Acceptor
{
public:
    Acceptor(const std::string& ip, unsigned short port);
    void ready();
    int  accept();
    int  fd();
    InetAddress getPeerInetAddress();
    ~Acceptor()=default;
private:
    void setReuseAddr();        //设置地址复用
    void setReusePort();        //设置端口复用
    void bind();
    void listen();
private:
    Socket _sock;
    InetAddress _addr;
    InetAddress _peerAddr;
};


#endif //REACTOR_ACCEPTOR_H
