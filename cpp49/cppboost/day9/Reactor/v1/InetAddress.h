/* @author chenchao @date @file @brief*/
#ifndef  _INETADDRESS_HPP
#define  _INETADDRESS_HPP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class InetAddress
{
public:
/* @file @brief @pram*/
    InetAddress(unsigned short& port,const std::string & ip = "127.0.0.0.1");
    InetAddress(const struct sockaddr_in& addr);
    //获取ip值
    std::string ip();
    //获取端口
    unsigned short port();
    struct sockaddr_in* getInetAddr();
    ~InetAddress()=default;
private:
    struct sockaddr_in _addr;
};
#endif   //_INETADDRESS_HPP

