//
// Created by chenchao on 24-5-8.
//

#ifndef REACTOR_SOKETIO_H
#define REACTOR_SOKETIO_H
class SocketIO
{
public:
    explicit SocketIO(int fd);
    ~SocketIO();
    //这里的char* 不仅仅表示是指向字符的指针，更表示1个字节。因为char类型在C/C++中的定义就是一个字节
    //而TCP套接字是流式的，无边界的传输方式，用字节来读取最合适
    int readn(char* buf, int len);
    int readLine(char* buf, int len);
    int writen(const char* buf, int len);
private:
    int _fd;
};
#endif //REACTOR_SOKETIO_H
