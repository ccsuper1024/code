/* @author chenchao @date @file @brief*/

#include "Socket.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
Socket::Socket()
{
    _fd = socket(AF_INET,SOCK_STREAM,0);
    if(_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Socket()\n");
}
Socket::Socket(int fd)
:_fd(fd)    //按照成员声明是的顺序构造，在构造派生类前会先构造基类对象
{
    fprintf(stdout,"Socket(int fd)\n");
}
int Socket::getFd() {
    return _fd;
}
Socket::~Socket() {
    close(_fd);
}
