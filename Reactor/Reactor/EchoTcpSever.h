//
// Created by chenchao on 24-5-10.
//

#ifndef REACTOR_ECHOTCPSEVER_H
#define REACTOR_ECHOTCPSEVER_H
#include "../Reactor/TcpConnection.h"
#include "../Reactor/TcpServer.h"
#include "../ThreadPool/ThreadPool.h"
#include <iostream>

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
class MyTask
{
public:
    MyTask(const std::string& msg, const TcpConnectionPtr& con)
    :_msg(msg)
    ,_con(con)
    {}
    void process() {
        //将业务逻辑在此处添加
        _con->sendLoop(_msg);
    }
private:
    std::string _msg;
    TcpConnectionPtr _con;
};

class EchoTcpSever
{
public:
    EchoTcpSever(size_t threadNum, size_t queSize, const std::string& ip, unsigned short port)
    : _server(ip, port)
    , _pool(threadNum, queSize)
    {}

    ~EchoTcpSever()=default;

    void start() {
        _pool.start();
        using namespace std::placeholders;
        _server.setAllCallback(std::bind(&EchoTcpSever::onNewConnection, this, _1),
                                std::bind(&EchoTcpSever::onMessage, this, _1),
                                std::bind(&EchoTcpSever::onClose, this, _1));
    }

    void stop() {
        _server.stop();
        _pool.stop();
    }
private:
    //建立新的连接时
    void onNewConnection(const TcpConnectionPtr& con) {
        std::cout << con->outputString() << "has connected" << std::endl;
    }

    //消息到达时
    void onMessage(const TcpConnectionPtr& con) {
        std::string msg = con->receive();       //接受客户的数据
        std::cout << ">> recv msg from client "<< msg << std::endl;

        //接受到客户端的msg之后，是可以进行业务的逻辑处理
        MyTask task(msg, con);
        //添加了Task，添加到线程中去了，执行了_taskQue;
        //在doTask()会执行此任务。如果要想客户端返回数据，就要在process中
        //处理数据，并把处理后的数据发回去,即调用_con->sendLoop(msg)
        _pool.addTask(std::bind(&MyTask::process, task));
    }
    //连接关闭时
    void onClose(const TcpConnectionPtr& con) {
        std::cout << con->outputString() << " has closed" << std::endl;
    }
private:
    TcpServer _server;
    ThreadPool _pool;
};

#endif //REACTOR_ECHOTCPSEVER_H
