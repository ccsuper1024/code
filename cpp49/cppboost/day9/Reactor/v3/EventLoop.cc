#include "EventLoop.h"
#include "TcpConnection.h"
#include "Acceptor.h"
#include <unistd.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <iostream>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor)
: _epfd(createEpollFd())
, _isLooping(false)
, _acceptor(acceptor)
, _evtList(1024)
{
    //将listenfd放在红黑树上进行监听
    addEpollReadFd(_acceptor.fd());
}

EventLoop::~EventLoop()
{
    close(_epfd);
}

//是否循环
void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::setNewConnectionCallback(TcpConnectionCallback &&cb)
{
    _onConnectionCb = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onCloseCb = std::move(cb);
}

//执行epoll_wait的函数
void EventLoop::waitEpollFd()
{
    int nready;
    do
    {
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 5000);
    }while(-1 == nready && errno == EINTR);

    if(-1 == nready)
    {
        cerr << "-1 == nready " << endl;
        return;
    }
    else if(0 == nready)
    {
        cout << ">>epoll_wait timeout" << endl;
    }
    else
    {
        //考虑扩容问题
        if(nready == (int)_evtList.size())
        {
            _evtList.resize(2 * nready);
        }

        for(int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            //新的连接
            if(fd == _acceptor.fd())//_acceptor.fd()就是listenfd
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleNewConnection();
                }
            }
            else//老的连接
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleMessage(fd);
                }
            }
        }
    }
}
//处理新的连接
void EventLoop::handleNewConnection()
{
    //执行accept函数
    int connfd = _acceptor.accept();
    if(connfd < 0)
    {
        perror("handleNewConnection");
        return;
    }
    //将accept函数返回的文件描述符放在红黑树上进行监听
    addEpollReadFd(connfd);

    //创建TcpConnection
    TcpConnectionPtr con(new TcpConnection(connfd));

    //回调机制
    //回调函数的注册
    con->setNewConnectionCallback(_onConnectionCb);//1、连接建立
    con->setMessageCallback(_onMessageCb);//2、消息到达
    con->setCloseCallback(_onCloseCb);//3、连接断开

    //将文件描述符与TcpConnection以键值对存放在map中
    _conns.insert(std::make_pair(connfd, con));

    //1、连接已经建立好了，就可以执行连接的建立事件
    con->handleNewConnectionCallback();
}

//处理消息的发送
void EventLoop::handleMessage(int fd)
{
    auto it = _conns.find(fd);
    if(it != _conns.end())
    {
        bool flag = it->second->isClosed();
        if(flag)
        {
            //3、连接断开的事件回调
            it->second->handleCloseCallback();
            //连接需要从红黑树上删除
            delEpollReadFd(fd);
            //将文件描述符从map中删除
            _conns.erase(it);
        }
        else
        {
            //2、消息到达的事件回调
            it->second->handleMessageCallback();
        }
    }
    else
    {
        cout << "该连接不存在" << endl;
    }
}

//创建文件描述符epfd
int EventLoop::createEpollFd()
{
    int fd = epoll_create(1);
    if(fd < 0)
    {
        perror("epoll_create");
        return fd;
    }
    return fd;
}

//将文件描述符放在红黑树上进行监听
void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret)
    {
        perror("addEpollReadFd");
        return;
    }
}

//将文件描述符从红黑树上删除
void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret)
    {
        perror("delEpollReadFd");
        return;
    }
}
