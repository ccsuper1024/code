//
// Created by chenchao on 24-5-9.
//
#ifndef REACTOR_EVENTLOOP_H
#define REACTOR_EVENTLOOP_H
#include <mutex>
#include <vector>
#include <map>
#include <memory>
#include <functional>

//前向声明
class Acceptor;
class TcpConnection;
class EventLoop
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
    using Functors = std::function<void()>;
public:
    EventLoop(Acceptor& acceptor);
    ~EventLoop();

    //开启事件循环
    void loop();        
    void unloop();

    void runInLoop(Functors&& cb);

    //在EventLoop中需要注册三个回调函数，将其传给TcpConnection
    void setNewConnectionCallback(TcpConnectionCallback&& cb);
    void setMessageCallback(TcpConnectionCallback&& cb);
    void setCloseCallback(TcpConnectionCallback&& cb);
private:
    //执行epoll_create 返回监听epoll event的内核文件的文件描述符
    int  createEpollFd();
    //向epoll event中添加要监听的文件描述符。红黑树和队列
    void addEpollReadFd(int fd);
    //将文件描述符从epoll监听的红黑树上删除
    void delEpollReadFd(int fd);
    //执行epoll_wait
    void waitEpollFd();
    //处理新连接
    void handleNewConnection();
    //发送消息
    void handleMessage(int fd);
private:
    //eventfd()创建事件，事件通知
    int createEventFd();
    //处理eventfd的读事件
    void handleRead();
    //处理eventfd中的写事件
    void wakeup();
    //将vector中的所有回调函数都进行执行
    void doPendingFunctors();
private:
    //epoll 事件的文件描述符
    int _epfd;
    //eventfd系统调用创建的文件描述符, 用于事件通知
    int _evfd;
    //标志事件循环开始
    bool _isLooping;
    Acceptor& _acceptor;        //引用成员必须在类成员初始化列表中初始化
    std::vector<struct epoll_event> _evtlist;
    std::map<int, std::shared_ptr<TcpConnection>> _conns;
    //陈硕说的，tcp的三个半事件中的三个，还有消息发送完毕
    TcpConnectionCallback _onConnectionCb;      //事件1：连接的建立
    TcpConnectionCallback _onMessageCb;         //事件2：消息的发送。包括同步阻塞发送，同步非阻塞发送和异步发送消息
    TcpConnectionCallback _onCloseCb;           //事件3：连接的关闭。应该包括主动关闭，被动关闭，和优雅关闭

    //待执行的任务的动态数组
    std::vector<Functors> _pendings;
    //锁
    std::mutex _mtx;
};
#endif //REACTOR_EVENTLOOP_H
