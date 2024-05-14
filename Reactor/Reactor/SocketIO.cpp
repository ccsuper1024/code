//
// Created by chenchao on 24-5-8.
//
//c standard library
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

//c++ standard library
#include <cstdio>

//C++ third party library
#include "SocketIO.h"
SocketIO::SocketIO(int fd)
:_fd(fd)
{

}
SocketIO::~SocketIO() {
    //close不是实际上删除socket内核文件，只会减小socket内核文件的引用计数，直到引用计数为0时才会真正的删除
    close(_fd);
}
//这里的char* 不仅仅表示是指向字符的指针，更表示1个字节。因为char类型在C/C++中的定义就是一个字节
//而TCP套接字是流式的，无边界的传输方式，用字节来读取最合适
int SocketIO::readn(char* buf, int len) {
    int left = len; //要读取的字节数
    char*ptr = buf; //指向分配内存中还未写入内容的地址
    int ret = 0;
    while(left > 0 ) {
        ret = read(_fd, ptr, left);
        if(ret == -1 && errno == EINTR)  {   //read系统调用被信号中断
            continue;
        } else if( ret == -1) {              //read系统调用出现错误
            fprintf(stderr, "read fail");
            //exit(EXIT_FAILURE);
            //TODO:LOG
            return len - left;
        } else if(0 == ret) {               //读取结束
            break;
        } else {
            ptr += ret;
            left -= ret;
        }
    }
    return len - left;  //返回还有多少指定的字节没读取出来
}
int SocketIO::readLine(char* buf, int len) {
    int left = len -1;
    char* ptr = buf;
    int ret = 0;
    int total = 0;

    while(left > 0) {
        //MSG_PEEK 不会将缓冲区的数据进行清空，只会进行拷贝操作
        //先从内核缓冲区中读取数据，但不改变内核缓冲区的文件指针。
        ret = recv(_fd, ptr, left, MSG_PEEK);
        if(-1 == ret && errno == EINTR) {       //被信号中断
            continue;
        }else if(-1 == ret) {                   //错误
            //TODO:LOG
            fprintf(stderr,"readLine error!");
        }else if(0 == ret) {                    //读取结束
            break;
        }else {                                 //对读取的数据进行判定，是否有换行符
            //先遍历读取到的数据，看其中是否有换行符
            //如果有换行符，则进行readn 来移动内核文件缓冲区中的文件指针
            //大小由idx决定。这样，内核文件输入缓冲区的文件指针只会移动一行大小
            for(int idx{0}; idx < ret; ++idx) {
                if(ptr[idx] == '\n') {
                    int sz = idx + 1;
                    readn(ptr, sz);
                    ptr += sz;
                    *ptr = '\0';    //c风格字符串以'\0'结尾

                    return total + sz;
                }
            }
            //只是为了移动内核文件中的文件指针
//            readn(ptr,ret);
            readn(buf,ret);
            total += ret;
            ptr += ret;
            left -= ret;
        }
    }
    *ptr = '\0';
    return total;           //返回实际读取的字节数
}
int SocketIO::writen(const char* buf, int len) {
    int left = len;
    const char* ptr = buf;
    int ret = 0;
    while (left > 0) {
        ret = writen(ptr, left);
        if(-1 == ret && errno == EINTR) {
            continue;
        } else if(-1 == ret) {
            fprintf(stderr, "write wrong");
            //TODO: log
            return len - left;
        } else if(0 == ret) {
            break;
        } else {
            ptr += ret;
            left -= ret;
        }
    }
    return len - left;      //返回实际写入的字节数
}
