//
// Created by chenchao on 24-5-8.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "Socket.h"
Socket::Socket()
{
    _fd = socket(AF_INET,SOCK_STREAM,0);
    if(_fd < 0) {
        //TODO: log
    }
}
Socket::Socket(int fd)
:_fd(fd)
{
    if(_fd < 1024) {
        //TODO: log
        fprintf(stderr, "%d is commonly used port", _fd);
        exit(EXIT_FAILURE);
    }
}
Socket::~Socket() {
    //关闭文件描述符， RAII机制
    close(_fd);
}
int Socket::id() {
    return _fd;
}

void Socket::shutDownWrite() {
    int ret = shutdown(_fd, SHUT_WR);
    if(ret < 0) {
        //TODO: log
        fprintf(stderr, "shutdown on writer fail");
        exit(EXIT_FAILURE);
    }
}
void Socket::shutDownRead() {
    int ret = shutdown(_fd, SHUT_RD);
    if(ret < 0) {
        //TODO: log
        fprintf(stderr, "shutdown on read fail");
        exit(EXIT_FAILURE);
    }
}
