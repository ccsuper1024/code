//
// Created by chenchao on 24-5-8.
//
#include <strings.h>
#include "InetAddress.h"
InetAddress::InetAddress()
{
    ::bzero(&_addr, sizeof(struct sockaddr_in));
}
InetAddress::InetAddress(const std::string& ip, unsigned short port)
{
    bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
}

//复制构造
InetAddress::InetAddress(const struct sockaddr_in& addr)
:_addr(addr)
{

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
