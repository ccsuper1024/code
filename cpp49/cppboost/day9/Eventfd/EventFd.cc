/* @author chenchao @date @file @brief*/
#include "EventFd.h"
#include <stdio.h>      //printf
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <poll.h>
#include <stdlib.h>
#include <cerrno>

EventFd::EventFd(EventFdCallback && cb)
:_evfd(createEventFd())
,_isStarted(false)
,_cb(std::move(cb))
{
    printf("EventFd(EventFdCallback && cb)\n");
}
//运行与停止
void EventFd::start(){
    //使用时间循环监听eventfd
    //poll 适用于少量连接，海量就绪。在内核中与select的区别是select的监听集合
    //是数组实现的位图，而poll在内核中的监听集合是一个链表。
    //所以poll没有监听文件描述符数量的限制，而select有
    struct pollfd pfd;
    pfd.fd = _evfd;
    pfd.events = POLLIN;

    _isStarted = true;
    while(_isStarted){
        int nready = poll(&pfd,1,5000); //timeout = 5000ms
        //A signal occurred before any requested event; see signal(7)
        //一个信号在任意被要求的事件发生前发生，即发生了软中断
        if(-1 ==nready && errno == EINTR){//软中断
            continue;   
        }else if(-1 == nready){           //poll失败
            fprintf(stderr,"-1 == nready");
            return ;    
        }else if(0 == nready){             //超时
            fprintf(stderr,"poll timeout");
        }else{                             //事件就绪
            if(pfd.events & POLLIN){    //可读
                //如果另外的线程发了通知，则调用handleRead()
                handleRead();
                if(_cb){
                    _cb();      //回调函数
                }
            }
        }
    }
}
void EventFd::stop(){
    _isStarted = false;
}

//该函数执行write，让B线程执行该函数
void EventFd::wakeup(){
    uint64_t one = 1;
    //写操作会将缓冲区写入的8字节整型值加入到内核计数器上。即会增加指定的8字节整数one，
    //到内核计数器上(eventfd系统调用的第一个参数)。如果此值达到0xfffffffffffffffe，
    //则阻塞(阻塞模式下)，知道该内核计数器被read。
    //向eventfd中写入一个uint64_t大小的数字
    //write的返回值是写入的字节大小
    ssize_t ret = write(_evfd,&one,sizeof(one));
    if(ret != sizeof(one)){
        perror("write");
        return;
    }    
}
//该函数中执行read,让A线程执行该函数
void EventFd::handleRead(){
    uint64_t one = 1;
    //读操作就是将counter值置0,即eventfd系统调用的第一个参数值，这个值由内核保存，如果是semophore就减1
    //如果内核计数器的值是不为0，读取成功，获得该值，并将内核计数器的值置为零
    //如果内核计数器的值为0，非阻塞模式下失败，errno置为EINVAL，阻塞模式下会一直阻塞到
    //内核计数器的值不为零。也就是另一个线程对此内核计数器(通过文件描述符)执行写操作
    //从eventfd中读出一个uint64_t大小的数字
    //read的返回值是写入的字节大小.read()是系统调用，会立即陷入内核
    ssize_t ret = read(_evfd,&one,sizeof(one));
    if(ret != sizeof(one)){
        perror("write");
        return;
    }    
}

//创建_eventFd;
int EventFd::createEventFd(){
    //eventfd（） 创建一个 “eventfd 对象”，该对象可以被用户空间应用程序用作事件
    //等待/通知机制，并且用于通知用户空间应用程序事件的内核。 该对象包含一个
    //无符号的 64 位整数 （uint64_t） 计数器由内核维护。 此计数器使用参数 initval 中
    //指定的值进行初始化。
    int fd = eventfd(10,0);
    if(fd < 0){
        perror("eventfd");
        exit(EXIT_FAILURE);
    }
    return fd;
}

