#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <vector>
#include <map>
#include <memory>
#include <functional>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;
using std::bind;

class Acceptor;//前向声明
class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

class EventLoop
{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();

    //是否循环
    void loop();
    void unloop();

    //在EventLoop中需要注册三个回调函数，将其传递给TcpConnection
    void setNewConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

private:
    //执行epoll_wait的函数
    void waitEpollFd();
    //处理新的连接
    void handleNewConnection();
    //处理消息的发送
    void handleMessage(int fd);
    //创建文件描述符epfd
    int createEpollFd();
    //将文件描述符放在红黑树上进行监听
    void addEpollReadFd(int fd);
    //将文件描述符从红黑树上删除
    void delEpollReadFd(int fd);

private:
    int _epfd;//红黑树上的节点
    bool _isLooping;//标识循环是否进行
    Acceptor &_acceptor;//为了调用Acceptor中的accept函数
    vector<struct epoll_event> _evtList;//存放满足条件的文件描述符
    map<int, TcpConnectionPtr> _conns;//存储文件描述符与TcpConnection的键值对

    TcpConnectionCallback _onConnectionCb;//1、连接建立
    TcpConnectionCallback _onMessageCb;//2、消息到达
    TcpConnectionCallback _onCloseCb;//3、连接断开


};

#endif
