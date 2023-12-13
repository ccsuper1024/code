/* @author chenchao @date @file @brief*/
#include "TcpConnection.h"
#include "SocketIO.h"
#include <vector>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
TcpConnection::TcpConnection(int fd)
:_sockio(std::make_unique<SocketIO>(fd))
{
    fprintf(stdout,"TcpConnection(int fd)\n");
}
void TcpConnection::send(const std::string& msg) {
    _sockio->writen(msg.c_str(),msg.size());
}
std::string TcpConnection::receive() {
    std::vector<char> vech;
    vech.reserve(65535);    //预留65535个char类的空间
    _sockio->readn(vech.data(),vech.size());

    return std::string(vech.begin(),vech.end());
}
TcpConnection::~TcpConnection() {
    fprintf(stdout,"~TcpConnection()\n");
}