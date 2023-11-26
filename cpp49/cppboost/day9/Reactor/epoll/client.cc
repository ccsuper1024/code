/* 
 * @file client.cc 
 * @brief tcp client test epoll
 * */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;

 
/* 
 * @brief
 * @param
 * @param  
 * @return 
 * */

int main()
{
    //创建socket内核文件
    int clntFd = socket(AF_INET,SOCK_STREAM,0);
    if(clntFd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    //初始化clinet sockaddr_int
    struct sockaddr_in clntAddr;
    memset(&clntAddr,0,sizeof(clntAddr));
    clntAddr.sin_family = AF_INET;
    clntAddr.sin_port = htons(8888);
    clntAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //客户端发起连接
    if(connect(clntFd,(struct sockaddr*)&clntAddr,sizeof(clntAddr))){
        perror("connect");
        close(clntFd);  //直接关闭。不会进行TIME_WAIT
        exit(EXIT_FAILURE);
    }
    printf("conn has connected!...\n");

    //echo server 上增加了一些业务逻辑
    while (1)
    {
        /* 不断从命令行读取数据并发送给服务端 */
        string line;
        cout << ">> pls input some message: ";
        getline(cin,line);
        line.append("\n");
        //客户端发送数据
        send(clntFd,line.data(),line.size(),0);
        char buff[128] = {0};
        //客户端接受数据
        recv(clntFd,buff,sizeof(buff),0);   //可以设置为非阻塞的。
        printf("recv msg from server: %s\n",buff);

    }
    //关闭套接字
    close(clntFd);
    
    return 0;
}

