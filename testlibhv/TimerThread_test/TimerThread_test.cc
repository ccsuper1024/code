/* @author chenchao @date @file @brief*/

#include <hv/singleton.h>
#include <iostream>
#include "TimerThread.h"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

namespace hv
{
class GlobalTimerThread
    : public TimerThread  //公开继承
{
    SINGLETON_DECL(GlobalTimerThread)
protected:      //给派生类继承的，对派生类是公开的，对派生类的对象和派生类的派生类是私有的
    GlobalTimerThread()
    :TimerThread()  //显示初始化类成员对象
    {}
    ~GlobalTimerThread()
    {}
public:
    static TimerID setTimeout(int timeout_ms,TimerCallback cb) {
        //此instance()函数是从SINGLETON_DECL宏定义而来
        return GlobalTimerThread::instance()->setTimer(timeout_ms,cb,1);
    }
    static void clearTimerout(TimerID timerID) {
        GlobalTimerThread::instance()->killTimer(timerID);
    }
    static TimerID setInterval(int interval_ms,TimerCallback cb) {
        //#define INFINITE    (uint32_t)-1
        return GlobalTimerThread::instance()->setTimer(interval_ms,cb,INFINITE);
    }
    static void clearInterval(TimerID timerID) {
        GlobalTimerThread::instance()->killTimer(timerID);
    }
};

SINGLETON_IMPL(GlobalTimerThread);
} // end of namespace hv

/* @brief @pram @return */
void test()
{
    hv::GlobalTimerThread::setTimeout(3000,[](hv::TimerID timerID) {
        fprintf(stdout,"setTimeout timerID=%lu time=%lus\n",
            static_cast<uint64_t>(timerID),
            //typedef unsigned long int __uint64_t;
            //typedef __uint64_t uint64_t;
            static_cast<uint64_t>(time(nullptr)));
    });
    hv::GlobalTimerThread::setInterval(1000,[](hv::TimerID timerID) {
        fprintf(stdout,"setInterval timerID=%lu time=%lus\n",
            static_cast<uint64_t>(timerID),
            static_cast<uint64_t>(time(nullptr)));
    });

    //press Enter to stop
    while(getchar() != '\n') {
        continue;
    }

    hv::GlobalTimerThread::exitInstance();
}
int main()
{
    test();
    return 0;
}
