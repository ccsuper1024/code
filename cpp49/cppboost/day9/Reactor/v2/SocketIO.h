/* @author chenchao @date @file 
    @brief:用于数据收发的类，封装了read函数和write操作。将真正的读写进行封装
*/
#ifndef  _SOCKIO_HPP
#define  _SOCKIO_HPP
#include <unistd.h>

class SocketIO
{
public:
/* @file @brief @pram*/
    explicit SocketIO(int fd);  //为避免内建类型隐式转换为SockIO对象，要对其进行禁止隐式转换
    ssize_t readn(char* buf,int len);
    ssize_t writen(const char* buf,int len);
    ssize_t readLine(char* buf,int len);
    int getFd();
    ~SocketIO();

private:
    int _fd;
};
#endif //_SOCKIO_HPP

