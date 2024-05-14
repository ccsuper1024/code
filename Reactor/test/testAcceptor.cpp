#include "../Reactor/Acceptor.h"
#include <iostream>
int main()
{
    Acceptor ac("127.0.0.1", 8888);
    ac.ready(); //启动服务器
    std::cout << "server is ready\n" << std::endl;
    std::cout << "And the fd is " << ac.fd() << std::endl;
    int connfd = ac.accept();
    std::cout << "the connfd is " << connfd << std::endl;
    std::cout << "the peeraddr is " << ac.getPeerInetAddress().ip() << std::endl;
    std::cout << "the peerport is " << ac.getPeerInetAddress().port() << std::endl;

    char str[6] = {'a','b'};
    send(connfd, str, sizeof(str), 0);

    return 0;
}