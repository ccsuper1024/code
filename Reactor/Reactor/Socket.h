//
// Created by chenchao on 24-5-13.
//

#ifndef REACTOR_SOCKET_H
#define REACTOR_SOCKET_H
#include "NoCopyable.h"

class Socket
        :public  NoCopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd() const;
    void shutDownWrite();
private:
    int _fd;
};


#endif //REACTOR_SOCKET_H
