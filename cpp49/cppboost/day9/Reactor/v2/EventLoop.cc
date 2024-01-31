/*  @file @brief @author chenchao @brief */
#include "EventLoop.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

/* @brief @pram @return */
EventLoop::EventLoop(Acceptor &acceptor)
:_epfd(creatEpollFd())
,_isLooping(false)
,_acceptor(acceptor)    //因为Acceptor的子对象不可复制，所以Acceptor也不可复制
,_evtList(1024)
,_onConnectionCb()
,_onMessageCb()
,_onCloseCb()
{
    //_evtList.reserve(128);
    //监听listenfd
    addEpollReadFd(_acceptor.getFd());
    fprintf(stdout,"EventLoop(Acceptor &acceptor\n");
}
EventLoop::~EventLoop() {
    close(_epfd);
    fprintf(stdout,"~EventLoop()\n");
}
void EventLoop::loop() {
    _isLooping = true;
    while (_isLooping)  {
        waitEpoolFd();
    }
    
}
void EventLoop::unloop() {
    _isLooping = false;
}
void EventLoop::setNewConnectionCallback(TcpConnectionCallback &&cb) {
    _onConnectionCb = std::move(cb);
}
void EventLoop::setMessageCallback(TcpConnectionCallback &&cb) {
    _onMessageCb = std::move(cb);
}
void EventLoop::setCloseCallback(TcpConnectionCallback &&cb) {
    _onCloseCb = std::move(cb);
}
void EventLoop::waitEpoolFd() {
    int ret;
    do {
        //超时5s
        ret =epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 5000);
    }while(-1 == ret && errno == EINTR);    //被信号中断

    if(-1 == ret) {              //系统调用出错
        perror("epoll_wait\n");
        exit(EXIT_FAILURE);
    }else if( 0 == ret) {        //超时
        perror("epoll wait time out");
        //exit(EXIT_FAILURE);
        //超时不应该退出，而应该继续循环
    }else {                      //监听就绪
        //考虑扩容
        if(ret == static_cast<int>(_evtList.size())) {
            _evtList.resize(2 * ret);    
        }
        //遍历内核返回的epoll_event事件的数组
        for(size_t idx = 0; idx < _evtList.size(); ++idx) {
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.getFd())  { //listenFd
                if(_evtList[idx].events & EPOLLIN) {//有新连接
                    handleNewConnection();      //处理新连接
                }
            } else {   //已经建立的连接
                if(_evtList[idx].events & EPOLLIN) {
                    handleMessage(_evtList[idx].data.fd);
                }
            }
                
        }
    }
}
//处理新的连接
void EventLoop::handleNewConnection() {
    //获取新连接的文件描述符
    int newconnfd = _acceptor.accept();
    if(newconnfd < 0 ) {
        perror("_acceptor.accept()\n");
        exit(EXIT_FAILURE);
    }
    //将新的连接描述符放到内核中epoll实例的红黑树上监听
    addEpollReadFd(newconnfd);

    //向TcpConnection对象传递此事件，以及必要的信息
    //创建TcpConnection
    TcpConnectionPtr con(new TcpConnection(newconnfd));

    //向TcpConnection对象注册回调
    con->setNewConnectionCallback(_onConnectionCb); //1.连接建立
    con->setMessageCallback(_onMessageCb);          //2.消息到达
    con->setCloseCallback(_onCloseCb);              //3.连接断开

    //将文件描述符与TcpConnection以键值对存放在map中
    _conns.insert(std::make_pair(newconnfd,con));

    //连接已经建立，可以执行连接的建立事件
    con->handleNewConnectionCallback();
    
}
//已经建立好的连接发送数据来了
void EventLoop::handleMessage(int fd) {
    //std::map<int,std::shared_ptr<TcpConnection>>::iterator
    auto it = _conns.find(fd);
    if(it != _conns.end())  {   //常用容器的尾置迭代器作为迭代器是否合理的依据
        bool flag = it->second->isClosed();
        if(flag) {
            //当连接已经断开时
            it->second->handleCloseCallback();
            //并把文件描述符从epoll实例的红黑树上删除
            delEpollReadFd(it->first);
            //再从map容器中删除
            _conns.erase(it);
            
        }else { 
            //当连接未关闭时
            it->second->handleMessageCallback();

        }
    }else { //此连接不在map容器中
        fprintf(stderr,"该连接不存在\n");

    }
}
int EventLoop::creatEpollFd() {
    int ret = epoll_create(1);
    if(ret < 0) {
        perror("epoll_create\n");
        exit(EXIT_FAILURE);
    }
    return ret;
}
void EventLoop::addEpollReadFd(int fd) {
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&evt);
    if(ret < 0) {
        perror("epoll_ctl\n");
        exit(EXIT_FAILURE);
    }
}
void EventLoop::delEpollReadFd(int fd) {
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&evt);
    if(ret < 0) {
        perror("epoll_ctl\n");
        exit(EXIT_FAILURE);
    }
}