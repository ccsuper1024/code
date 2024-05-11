#include "../Reactor/InetAddress.h"
#include <iostream>

int main()
{
    InetAddress addr("127.0.0.1", 8888);
    std::cout << "ip = " << addr.ip() << std::endl;
    std::cout << "port = " << addr.port() << std::endl;

}