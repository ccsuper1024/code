/* @author chenchao 
    @date 
    @file 
    @brief 通过文件描述符来通知计时器过期事件
    */
#ifndef  _TIMERFD_HPP
#define  _TIMERFD_HPP

#include <functional>

using std::function;

class TimerFd
{
    using TimerFdCallback = function<void()>;
public:
    /* @file @brief @pram*/
    explicit TimerFd(int initSec,int peridocSec,TimerFdCallback&& cb); //防止隐式构造
    ~TimerFd();

    //运行与停止
    void start();
    void stop();
private:
    //在该函数中执行read，让A线程执行该函数
    void handleRead();

    //创建timerfd
    int createTimerFd();

    //创建定时器
    void setTimerFd(int initSec,int peridocSec);

private:
    int _timerfd;       //timerfd_create系统调用创建的文件描述符
    int _initSec    ;       //定时器的初始事件
    int _peridocSec;    //定时器的周期时间
    bool _isStarted;    //标识TimerFd是否在运行
    TimerFdCallback _cb;//定时器任务
};
#endif      //_TIMERFD_HPP

