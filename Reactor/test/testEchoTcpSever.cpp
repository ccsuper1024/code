//
// Created by chenchao on 24-5-10.
//
#include "../Reactor/EchoTcpSever.h"

int main() {
    EchoTcpSever server(4,10,"127.0.0.1", 8888);
    server.start();
    server.stop();
    return 0;
}