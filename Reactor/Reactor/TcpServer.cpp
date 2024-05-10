//
// Created by chenchao on 24-5-8.
//

#include "TcpServer.h"
TcpServer::TcpServer(const std::string& ip, unsigned short port)
: _acceptor(ip,port)
, _loop(_acceptor)
{

}
TcpServer::~TcpServer() {}
//让服务器运行起来
void TcpServer::start() {
    _acceptor.ready();
    _loop.loop();
}
//停止服务器
void TcpServer::stop() {
    _loop.unloop();
}
//对三个半事件注册回调函数
void TcpServer::setAllCallback(TcpConnectionCallback&& onConnection,
                    TcpConnectionCallback&& onMessage,
                    TcpConnectionCallback&& onClose) {

    _loop.setNewConnectionCallback(std::move(onConnection));
    _loop.setMessageCallback(std::move(onMessage));
    _loop.setCloseCallback(std::move(onClose));
}
