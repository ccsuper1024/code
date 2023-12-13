/* @author chenchao @date @file @brief*/
#include "TcpConnection.h"
#include "Acceptor.h"
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
void test(){
    Acceptor acpt(8888);
    acpt.ready();       //服务器开启监听
    
    //建立连接
    TcpConnection conn(acpt.accept());

    //调试
    std::string str = "hello";  //复制构造
    conn.send(str);

    while(true) {
        cout << ">>recv msg from client: " << conn.receive() << endl;
        conn.send("hello!\n");
    }
}
int main()
{
    test();
    return 0;
}

