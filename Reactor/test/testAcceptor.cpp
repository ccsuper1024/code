#include "../Reactor/Acceptor.h"
#include <iostream>
int main()
{
    Acceptor ac("127.0.0.1", 8888);
    ac.ready(); //启动服务器
    std::cout << "server is ready\n" << std::endl;
    std::cout << "And the fd is " << ac.fd() << std::endl;
    std::cout << "the connfd is " << ac.accept() << std::endl;
    std::cout << "the peeraddr is " << ac.getPeerInetAddress().ip() << std::endl;
}