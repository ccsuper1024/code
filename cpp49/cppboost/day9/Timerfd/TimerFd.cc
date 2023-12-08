/* @author chenchao @date @file @brief*/
#include "TimerFd.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <poll.h>
#include <sys/timerfd.h>
#include <iostream>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

/* @file @brief @pram*/
TimerFd::TimerFd(int initSec, int peridocSec, TimerFdCallback &&cb)
    : _timerfd(createTimerFd()), _initSec(initSec), _peridocSec(peridocSec), _isStarted(false), _cb(std::move(cb))
{
    printf("TimerFd(int initSec,int peridocSec,TimerFdCallback &&cb)\n");
}
TimerFd::~TimerFd(){
    setTimerFd(0,0);
    close(_timerfd);
}
// 运行与停止
void TimerFd::start()
{
    _isStarted = true;
    /*
        //创建IO复用机制，监听timerfd_create返回的文件描述符
        //使用epoll适合海量连接，少量就绪。而定时器事件不需要太多的连接，其实poll更适合，甚至select也行
        //创建epoll实例
        int efd = epoll_create1(0);     //等同于epoll_create(0);//epoll_create(int size)中size参数已经不再采用
        if(efd == -1){
            perror("epoll_create1(0)\n");
            close(_timerfd);
            exit(EXIT_FAILURE);
        }
        struct epoll_event ev;
        ev.data.fd = _timerfd;
        ev.events = EPOLLIN;   //只监听读就绪

        //把epoll实例加入内核监听
        int ret = epoll_ctl(efd,EPOLL_CTL_ADD,_timerfd,&ev);
        if(ret == -1){
            perror("epoll_ctl\n");
            close(_timerfd);
            close(efd);
            exit(EXIT_FAILURE);
        }

        struct epoll_event* evtList = new epoll_event[1024];
        //设置定时器
        setTimerFd(_initSec,_peridocSec);
        //事件循环
        while(_isStarted){
            int nready = epoll_wait(efd,evtList,1024,5000);
            if(-1 == nready && errno == EINTR){     //发生软中断
                continue;
            }else if(-1 == nready){                 //发生错误
                fprintf(stderr,"-1 == nready\n");
                exit(EXIT_FAILURE);
            }else if(0 == nready){                  //超时错误
                fprintf(stderr,"epoll timeout");
                exit(EXIT_FAILURE);
            }else{                                   //文件描述符就绪
                //遍历内核返回给用户程序的evtList数组
                for(int idx = 0; idx < 1024;++idx){
                    //文件描述符就绪情况下。若此文件描述符是_timerfd,且发生的时间是EPOLLIN事件
                    if(evtList[idx].data.fd == _timerfd &&
                    (evtList[idx].events & EPOLLIN)){
                        handleRead();
                        if(_cb){
                            _cb();                    //执行回调
                        }
                    }
                }
            }
        }*/
    struct pollfd pfd;
    pfd.fd = _timerfd;
    pfd.events = POLLIN;

    // 需要设置定时器
    setTimerFd(_initSec, _peridocSec);

    while (_isStarted)
    {
        int nready = poll(&pfd, 1, 5000);
        if (-1 == nready && errno == EINTR) //软中断
        {
            continue;
        }
        else if (-1 == nready)              //错误
        {
            cerr << "-1 == nready" << endl;
            return;
        }
        else if (0 == nready)               //超时
        {
            cout << ">>poll timeout" << endl;
        }
        else                                //事件就绪
        {
            if (pfd.events & POLLIN)
            {
                // 定时器所在线程触发了定时器事件，就会被poll监听到。
                // 执行handleRead，再就可以执行回调了
                handleRead();
                if (_cb)
                {
                    _cb(); // 回调函数的执行
                }
            }
        }
    }
}
void TimerFd::stop()
{
    if (_isStarted)
    {
        _isStarted = false;
        setTimerFd(0, 0);
    }
}

// 在该函数中执行read，让A线程执行该函数
void TimerFd::handleRead()
{
    uint64_t one = 1;
    ssize_t ret = read(_timerfd, &one, sizeof(uint64_t));
    if (ret != sizeof(uint64_t))
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
}

// 创建timerfd
int TimerFd::createTimerFd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd < 0)
    {
        perror("TimerFd::create");
        exit(EXIT_FAILURE);
    }
    return fd;
}

// 创建定时器
void TimerFd::setTimerFd(int initSec, int peridocSec)
{
    if(initSec == 0 && peridocSec == 0){
        return;
    }
    struct itimerspec value;
    value.it_value.tv_sec = initSec; // 起始时间的秒数
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = peridocSec; // 间隔时间的秒数
    value.it_value.tv_nsec = 0;

    // timerfd_settime启动或者停止文件描述符引用的定时器
    // 第一个参数是指向定时器的文件描述符
    // 第二个参数可以是TFD_TIMER_ABSTIME或者TFD_TIMER_CANCEL_ON_SET，为0则是相对定时，
    // 第三个参数是定时器定时的时间
    // 第四个参数是定时器上一次使用的定时时间，可以为空
    int ret = timerfd_settime(_timerfd, 0, &value, nullptr);
    if (ret)
    {
        perror("timerfd_settime\n");
        close(_timerfd);
        exit(EXIT_FAILURE);
    }
}
