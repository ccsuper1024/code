/* @author chenchao @date @file @brief*/
#ifndef  _ACCEPTOR_HPP
#define  _ACCEPTOR_HPP
#include "Socket.h"
#include "InetAddress.h"

class Acceptor
{
public:
/* @file @brief @pram*/
    Acceptor(unsigned short port,const std::string & ip="127.0.0.1",int backlog=128);
    /*Acceptor(const Acceptor& acceptor) 
    :_sock(acceptor._sock.getFd())
    ,_addr(acceptor._addr)
    ,_backlog(acceptor._backlog)
    {}*/
    ~Acceptor();
    void ready();
    int accept();
    int getFd();
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
    
private:
    Socket _sock;           //监听文件描述符
    InetAddress _addr;      //服务端的地址
    int _backlog;           //listen()调用后，内核文件对象中半连接队列和全连接队列之和
};
#endif  //_ACCEPTOR_HPP

