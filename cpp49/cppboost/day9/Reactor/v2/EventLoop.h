/*  @file @brief @author chenchao @brief */
#ifndef  _EVENETLOOP_HPP
#define  _EVENETLOOP_HPP
#include "Acceptor.h"
#include "TcpConnection.h"
#include <vector>
#include <map>
#include <memory>
#include <functional>

//这个using指令必须写在全局中，或者类的Public区域供外界调用。
//写在类的Public区域要加类名以及作用域控制符作为限制
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
class EventLoop
{
    
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
public:
/* @file @brief @pram*/
    EventLoop(Acceptor& acceptor);
    ~EventLoop();
    void loop();
    void unloop();
    void setNewConnectionCallback(TcpConnectionCallback&& cb);
    void setMessageCallback(TcpConnectionCallback&& cb);
    void setCloseCallback(TcpConnectionCallback&& cb);
private:
    void waitEpoolFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int creatEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);
private:
    int _epfd;
    bool _isLooping;
    Acceptor& _acceptor;
    std::vector<struct epoll_event> _evtList;
    std::map<int,TcpConnectionPtr> _conns;
    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};
#endif  //_EVENETLOOP_HPP

