/* @author chenchao @date @file @brief*/
#ifndef  HV_TIMER_THREAD_HPP
#define  HV_TIMER_THREAD_HPP

#include <hv/EventLoopThread.h>
namespace hv
{
class TimerThread
:public EventLoopThread
{
public:
/*  @pram
    @return 
*/
    std::atomic<TimerID> nextTimerID;
    TimerThread()
    :EventLoopThread() {
        nextTimerID = 0;
        EventLoopThread::start();
    }
    //虚析构函数
    virtual ~TimerThread() {
        EventLoopThread::stop();
        EventLoopThread::join();
    }
public:
    //setTimer setTimout killTimer resetTimer thread-safe
    //#define INFINITE    (uint32_t)-1
    //typedef unsigned int __uint32_t;
    //typedef __uint32_t uint32_t;
    TimerID setTimer(int timeout_ms, TimerCallback cb,uint32_t repeat = INFINITE){
        TimerID timerID = ++nextTimerID;
        loop()->setTimerInLoop(timeout_ms,cb,repeat,timerID);
        return timerID;
    }

    //alias javascript setTimerout, setInterval
    TimerID setTimeout(int timeout_ms,TimerCallback cb) {
        return setTimer (timeout_ms,cb,1);
    }
    TimerID setInterval(int internal_ms,TimerCallback cb) {
        return setTimer(internal_ms,cb,INFINITE);
    }

    void killTimer(TimerID timerID) {
        loop()->killTimer(timerID);
    }

    void resetTimer(TimerID timerID, int timeout_ms = 0) {
        loop()->resetTimer(timerID,timeout_ms);
    }

};  //end of class TimerThread
} //end of namespace hv

#endif  //HV_TIMER_THREAD_HPP

