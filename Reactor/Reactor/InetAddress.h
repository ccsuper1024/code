//
// Created by chenchao on 24-5-8.
//
#ifndef REACTOR_INETADDRESS_H
#define REACTOR_INETADDRESS_H
#include <arpa/inet.h>
#include <string>

class InetAddress
{
public:
    InetAddress();
    InetAddress(const std::string& ip, unsigned short port);
    InetAddress(const struct sockaddr_in& addr);
    bool isEmpty();
    std::string ip() const;
    unsigned short port() const;
    const struct sockaddr_in* getInetAddrPtr() const;
    ~InetAddress()=default;
private:
    struct sockaddr_in _addr;
};


#endif //REACTOR_INETADDRESS_H
