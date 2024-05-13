//
// Created by chenchao on 24-5-13.
//
#include "Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

Socket::Socket()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0) {
        perror("socket\n");
        return ;
    }
}
Socket::Socket(int fd)
:_fd(fd)
{

}
Socket::~Socket() {
    close(_fd);
}
int Socket::fd() const {
    return _fd;
}
void Socket::shutDownWrite() {
    int ret = shutdown(_fd, SHUT_WR);
    if(ret) {
        perror("shutdown\n");
        return;
    }
}
