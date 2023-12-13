/* @author chenchao @date @file @brief*/
#include "SocketIO.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
//为避免内建类型隐式转换为SockIO对象，要对其进行禁止隐式转换
SocketIO::SocketIO(int fd) 
:_fd(fd)
{
    fprintf(stdout,"explicit SocketIO(int fd)\n");
} 
ssize_t SocketIO::readn(char* buf,int len) {
    int left = len;
    char* pstr = buf;
    int ret = 0;

    while(left > 0) {
        ret = read(_fd,pstr,left);
        if(-1 == ret && errno == EINTR) {  //被其他信号中断
            continue;
        } else if(-1 == ret) {              //read失败
            perror("read error -1\n");
            exit(EXIT_FAILURE);
        } else if(0 == ret) {               //read succeed
            break;
        } else {                            //读取成功但是读取的字节数不比指定的len大
            pstr += ret;
            left -= ret;
        }     
    }
    return len - left;
}
ssize_t SocketIO::writen(const char* buf,int len) {
    int left = len - 1;     //最后一个字符不需要写入,这传入的是一个c风格字符串？
    const char* pstr = buf;
    int ret = 0;

    while(left > 0) {
        ret = write(_fd,pstr,left);
        if(-1 == ret && errno == EINTR) {           //被信号打断
            continue;
        } else if( -1 == ret) {                     //write system call call wrong
            perror("written error -1\n");
            exit(EXIT_FAILURE);
        } else if(0 == ret) {                       //write succeed
            break;
        } else {
            pstr += ret;
            left -= ret;
        }          
    }
    return len - left;
}
ssize_t SocketIO::readLine(char* buf,int len) {
    int left = len - 1;
    char* pstr = buf;
    int ret = 0, total = 0;

    while (left > 0) {
        //MSG_PEEK不会将缓冲区中的数据进行清空，只会进行拷贝操作。也就是不会移动内核文件对象的缓冲区指针
        //recv()对基于流的文件描述符，如SOCK_STREM和基于消息的文件描述符，如SOCK_DGRAM
        //的操作不一样
        ret = recv(_fd,pstr,left,MSG_PEEK);
        if(-1 == ret && errno == EINTR) {
            continue;                           //被信号中断时
        } else if(-1 == ret) {
            perror("readLine error -1\n");      //recv()错误
            exit(EXIT_FAILURE); 
        } else if(0 == ret) {
            break;                              //recv succeed
        } else {                                //0 < ret < len
            for(int idx = 0; idx < ret ; ++idx) {
                if(pstr[idx] == '\n') {     //recv到了内核缓存区中的一行(此处的一行是以'\n',换行符作为标志的)
                    int sz = idx + 1;
                    readn(pstr,sz);     //将内核文件缓冲区中ret字节数读取出来。注意，不是向recv一样拷贝，而是从内核缓冲区中删去ret个字节数
                    pstr += sz;
                    *pstr = '\0';       //c风格字符串以空字符结尾
                    return total + sz;  //返回总的读取的字节数
                }
            }
            //当读取的字节数中没有'\n'，也就是不是一行时
            readn(pstr,ret);  //从内核态拷贝到用户态，会导致_fd指向的内核文件对象的缓冲区的指针移动
            total += ret;
            pstr += ret;
            left -= ret;
        }

    }
    *pstr = '\0';

    return total - left;  //一个负数？
}
SocketIO::~SocketIO()
{
    close(_fd);     //关闭连接文件描述符
    fprintf(stdout,"~SocketIO()\n");
}
