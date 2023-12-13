/* @author chenchao @date @file @brief*/
#ifndef  _TCPCONNECTION_HPP
#define  _TCPCONNECTION_HPP
#include <memory>
//前向声明
class SocketIO;
class TcpConnection
{
public:
/* @file @brief @pram*/
    explicit TcpConnection(int fd);
    void send(const std::string& msg);
    std::string receive();
    ~TcpConnection();

private:
    std::unique_ptr<SocketIO> _sockio;  //每个TcpConnection都独占一个连接文件描述符
};
#endif      //_TCPCONNECTION_HPP

