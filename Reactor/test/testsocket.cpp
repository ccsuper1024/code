#include <iostream>
#include "../Reactor/Socket.h"

int main() {
    Socket sock;
    std::cout << "the id of the sock is " << sock.fd() << std::endl;
}