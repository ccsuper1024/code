/* @author chenchao @date @file @brief*/
#ifndef  _SOCKET_HPP
#define  _SOCKET_HPP
#include "Nocopyable.h"
class Socket
:public Nocopyable  //公开继承禁止复制基类
{
public:
/* @file @brief @pram*/
    Socket();
    explicit Socket(int fd);
    int getFd();
    ~Socket();
private:
    int _fd;
};
#endif //_SOCKET_HPP

