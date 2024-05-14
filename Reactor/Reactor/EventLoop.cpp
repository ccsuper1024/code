//
// Created by chenchao on 24-5-8.
//
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"


EventLoop::EventLoop(Acceptor& acceptor)
:_epfd(createEpollFd())
,_evfd(createEventFd())
,_isLooping(false)
,_acceptor(acceptor)    //复制构造
,_evtlist(1024)
,_conns()
,_onConnectionCb()
,_onMessageCb()
,_onCloseCb()
,_pendings()
,_mtx()
{
    //监听 监听套件字描述符
    addEpollReadFd(_acceptor.fd());
    //监听eventfd创建的文件描述符
    addEpollReadFd(_evfd);
}
EventLoop::~EventLoop() {
    //回收系统资源
    close(_epfd);
    close(_evfd);
}
void EventLoop::loop() {    //开启事件循环
    _isLooping = true;
    while(_isLooping) {
        waitEpollFd();
    }
}
void EventLoop::unloop() {
    _isLooping = false;
}
void EventLoop::runInLoop(Functors&& cb) {
   /*
    将要执行的任务先存放在vector中
    任务就是线程池处理好之后的msg
    以及发送msg的能力的TcpConnection中的send函数
   */
    {
        std::lock_guard<std::mutex> lk(_mtx);
        _pendings.push_back(std::move(cb));
    }

    wakeup();
}
void EventLoop::handleNewConnection() {
    //执行accept函数。accept系统调用是阻塞等待socket系统调用创建的内核文件中的全连接队列
    //它会从中拿出一个新连接，并创建一个相应的内核文件(具备输入输出缓冲区)。
    //所以每次epoll监听到监听套件字就绪时，也就意味着有新连接到来了
    int connfd = _acceptor.accept();
    if( connfd < 0) {
        fprintf(stderr, "_acceptor.accept() error\n");
        //TODO:LOG
        return ;
    }

    //accept函数返回的文件描述符放在红黑树上监听
    addEpollReadFd(connfd);

    //创建TcpCconnection用于读写
    //此处构建std::shared_ptr版本的TcpConnection。需要继承std::enable_shared_from_this<>;
    TcpConnectionPtr con(new TcpConnection(connfd ,this));

    //向TcpConnection中注册回调函数
    con->setNewConnectionCallback(_onConnectionCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    //将文件描述符与TcpConnection以键值对形式存放在std::map中，方便下一次的通信
    _conns.insert(std::make_pair(connfd, con));

    //新的连接建立了，调用TcpConnection处理新连接的函数
    con->handleNewConnectionCallback();

}
void EventLoop::handleMessage(int fd) {
    auto it = _conns.find(fd);  //找到旧连接
    if(it != _conns.end()) {
        bool flag = it->second->isClosed();
        if(flag) {          //如果连接断开
            //连接断开时的事件回调
            it->second->handleCloseCallback();
            //将连接对应的文件描述符从epoll的红黑树上溢出
            delEpollReadFd(fd);
            //从std::map中删除断开的连接
            _conns.erase(it);
        } else {            //如果连接畅通
            //消息到达时，回调事件
            it->second->handleMessageCallback();
        }
    } else {
        fprintf(stdout, "该连接不存在\n");
        //TODO:LOG
        //或者尝试建立新连接？？？
    }
}
int EventLoop::createEpollFd() {
    int fd = epoll_create(1);
    if(fd < 0) {
        fprintf(stderr, "epoll_create error\n");
        //TODO:LOG
        return fd;
    }
    return fd;
}
void EventLoop::addEpollReadFd(int fd) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;        //epoll默认为水平触发
    int ret = epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
    if (ret < 0) {
        fprintf(stderr, "addEpollFd error\n");
        //TODO:LOG
        return ;
    }
}
void EventLoop::delEpollReadFd(int fd) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(fd,EPOLL_CTL_DEL,fd,&ev);
    if (ret < 0) {
        fprintf(stderr, "delEpollFd error\n");
        //TODO:LOG
        return ;
    }

}
void EventLoop::waitEpollFd() {
    int nready;
    do {
        nready = epoll_wait(_epfd, &(*_evtlist.begin()),
                            _evtlist.size(), 5000);     //timeout为-1则永久等待
    }while(-1 == nready && EINTR == errno);     //被信号中断

    if(-1 == nready) {                          //出现错误
        fprintf(stderr, "-1 == ready\n");
        //TODO:LOG
        return;
    } else if(0 == nready) {                    //在指定时间内没有事件就绪
        fprintf(stderr, "epoll_wait timeout\n");
        //TODO:LOG
        //只是一次超时，在事件循环中不需要退出程序
    } else {                                    //事件就绪
        //考虑就绪的事件可能越来越多，会造成vector移动内存，所以预先扩容
        //设定预先扩容的边界是就绪事件数量大于当前大小的0.7，小于当前数量大小时
        if(nready >= static_cast<int>(_evtlist.size() * (7 / 10)) ||
                nready <= static_cast<int>(_evtlist.size())) {
            _evtlist.resize(2 * nready);
        }
        for(long unsigned int idx{0}; idx < _evtlist.size(); ++idx) {     //遍历返回的事件列表
                int fd = _evtlist[idx].data.fd;
                auto event = _evtlist[idx].events;
                //就绪事件是监听套接字时
                if(fd == _acceptor.accept()) {
                    if(event & EPOLLIN) {    //读事件就绪
                        handleNewConnection();
                    }
                } else if(fd == _evfd) {                    //eventfd创建的套接字就绪时
                    if(event & EPOLLIN) {
                        handleRead();
                        //将所有任务全部执行一下
                        //线程池处理好之后的msg以及发送数据
                        //能力的TcpConnection中的send
                        doPendingFunctors();
                    }
                } else {                                    //旧的连接
                    if(event & EPOLLIN) {
                        handleMessage(fd);
                    }
                }
        }
    }
}
void EventLoop::setNewConnectionCallback(TcpConnectionCallback&& cb) {
    _onConnectionCb = std::move(cb);
}
void EventLoop::setMessageCallback(TcpConnectionCallback&& cb) {
    _onMessageCb = std::move(cb);
}
void EventLoop::setCloseCallback(TcpConnectionCallback&& cb) {
    _onCloseCb = std::move(cb);
}
int EventLoop::createEventFd() {
    int fd = eventfd(10, 0);
    if (fd  == -1) {
        fprintf(stderr, "eventfd error\n");
        //TODO:LOG
        exit(EXIT_FAILURE);
    }
    return fd;
}
void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t ret = write(_evfd, &one, sizeof(uint64_t));
    if(ret != sizeof(uint64_t)) {
        fprintf(stderr, "eventfd write error\n");
        //TODO:LOG
        exit(EXIT_FAILURE);
    }
}
void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t ret = read(_evfd, &one, sizeof(uint64_t));
    if(ret != sizeof(uint64_t)) {
        fprintf(stderr, "eventfd read error\n");
        //TODO:LOG
        exit(EXIT_FAILURE);
    }
}
//将vector中的所有回调函数都进行执行
void EventLoop::doPendingFunctors() {
    std::vector<Functors> tmp;
    //将std::vector中的cb取出来，放在另外的容器中进行变量
    //那么原来的std::vector可以释放出来，可以往其中写数据
    {
        std::lock_guard<std::mutex> lk(_mtx);
        tmp.swap(_pendings);
    }
    //遍历vector，将所有callback执行
    for(auto& cb: tmp) {
        cb();
    }
}

