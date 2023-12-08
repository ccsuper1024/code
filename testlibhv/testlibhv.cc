/*  @author chenchao 
    @date 
    @file 
    @brief
*/

#include <stdio.h>
#include <iostream>
#include <hv/TcpServer.h>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

using namespace hv;
/* @brief @pram @return */

int main(int argc, char* argv[])
{
    if(argc < 2){
        fprintf(stderr,"Usage: %s port\n",argv[0]);
        return -10;
    }
    int port = atoi(argv[1]);
    TcpServer serv;
    int listenfd = serv.createsocket(port);
    if(listenfd < 0){
        return -20;
    }
    printf("server listen on port %d ,listenfd = %d ...\n",port,listenfd);
    serv.onConnection = [](const SocketChannelPtr& channel){    //智能指针对象的引用
        std::string peeraddr = channel->peeraddr(); //获取新连接的客户地址
        if(channel->isConnected()){
            fprintf(stderr,"%s connected! connfd = %d\n",peeraddr.c_str(),channel->fd());
        }else{
            fprintf(stderr,"%s disconnected! connfd=%d\n",peeraddr.c_str(),channel->fd()); 
        }
    };
    serv.onMessage = [](const SocketChannelPtr& channel,Buffer* buf){
        //echo
        fprintf(stdout,"%.*s\n",static_cast<int>(buf->size()),
                    static_cast<char*>(buf->data()));
        channel->write(buf);
    };
    serv.onWriteComplete = [](const SocketChannelPtr& channel,Buffer* buf){
        fprintf(stdout,"> %.*s\n",static_cast<int>(buf->size()),
                        static_cast<char*>(buf->data()));
    };
    serv.setThreadNum(4);
    serv.start();

    while (true)
    {
        /* code */
        sleep(1);
    }
    
    return 0;
}

