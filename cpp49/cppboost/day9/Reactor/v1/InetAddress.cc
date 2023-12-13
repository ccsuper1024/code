/* @author chenchao @date @file @brief*/
#include "InetAddress.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
InetAddress::InetAddress(unsigned short& port,const std::string & ip) 
{   
    ::bzero(&_addr,sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    fprintf(stdout,"InetAddress(unsigned short& port,const std::string & ip)\n");
}
InetAddress::InetAddress(const struct sockaddr_in& addr) 
:_addr(addr)
{
    fprintf(stdout,"InetAddress(const struct sockaddr_in& addr)\n");
}
//获取ip值,此处返回值不可以是引用类型，因为引用类型不可引用临时对象
std::string InetAddress::ip() {     //RVO--return value optimize
    //将网络字节序(大端)转成主机字节序(小端(x86))
    //并且把数字转成字符串
    return std::string(inet_ntoa(_addr.sin_addr));
}
//获取端口
unsigned short InetAddress::port() {
    //将网络字节序(大端)转成主机字节序(小端(x86))
    return ntohs(_addr.sin_port);
}
struct sockaddr_in* InetAddress::getInetAddr() {
    return &_addr;
}