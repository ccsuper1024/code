//
// Created by chenchao on 24-5-8.
//

#ifndef REACTOR_SOCKET_H
#define REACTOR_SOCKET_H

class Socket
{
public:
    Socket();
    explicit  Socket(int fd);
    ~Socket();
    int fd();
    void shutDownWrite();
    void shutDownRead();
private:
    int _fd;
};
#endif //REACTOR_SOCKET_H
