/* @author chenchao 
   @date 
   @file 
   @brief create a file descriptor for event notification between process/thread and process/thread
*/
#ifndef  _EVENTFD_HPP
#define  _EVENTFD_HPP
#include <functional>

using std::function;
using std::bind;
using std::move;

class EventFd
{
    using EventFdCallback = function<void()>;
public:
    /* @file @brief @pram*/
    //explicit关键字防止隐式构造
    explicit EventFd(EventFdCallback && cb);
    ~EventFd()=default;

    //运行与停止
    void start();
    void stop();

    //该函数执行write，让B线程执行该函数
    void wakeup();
private:
    //该函数中执行read,让A线程执行该函数
    void handleRead();

    //创建_eventFd;
    int createEventFd();
private:
    int _evfd;      //eventfd()系统调用创建的文件描述符
    bool _isStarted;
    EventFdCallback _cb;
};
#endif      //_EVENTFD_HPP

