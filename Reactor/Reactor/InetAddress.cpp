//
// Created by chenchao on 24-5-8.
//
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "InetAddress.h"

//默认构造
InetAddress::InetAddress()
{
    ::bzero(&_addr, sizeof(struct sockaddr_in));
}
InetAddress::InetAddress(const std::string& ip, unsigned short port)
{
    bzero(&_addr, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());  //主机字节序->网络字节序
    _addr.sin_port = htons(port);
}

//复制构造
InetAddress::InetAddress(const struct sockaddr_in& addr)
:_addr(addr)    //结构体复制
{

}

bool InetAddress::isEmpty() {
    return _addr.sin_family == 0 && _addr.sin_port == 0
             && _addr.sin_addr.s_addr == 0;
}
std::string InetAddress::ip() const {
    return std::string(inet_ntoa(_addr.sin_addr));
}
unsigned short InetAddress::port() const {
    return ntohs(_addr.sin_port);
}
const struct sockaddr_in* InetAddress::getInetAddrPtr() const{
    return &_addr;
}
