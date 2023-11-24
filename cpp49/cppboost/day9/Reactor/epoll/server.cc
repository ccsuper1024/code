/* @file server.cc 
   @brief tcp server ,test epoll
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h> //socket(),bind(),listen(),accept(),connect(),sockpair(),setsockopt()等
#include <arpa/inet.h>/* Convert Internet host address from numbers-and-dots notation in CP into binary data in network byte order.  */ //extern in_addr_t inet_addr (const char *__cp) __THROW;
#include <netinet/in.h> //htons struct in_addr {in_addr_t s_addr;}; INADDR_ANY
#include <iostream>
#include <string>


using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;

void errorHandling(int fd,string str){
    if(fd < 0){
        perror(str.c_str());
        exit(EXIT_FAILURE);
    }
}
/* @brief @param @param   @return */
int main()
{
    //创建socket内核文件
    int servFd = socket(AF_INET,SOCK_STREAM,0);
    errorHandling(servFd,"socket");
    
    //设置socket内核文件的属性
    //地址复用
    int opt = 1;
    int ret = setsockopt(servFd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt));
    errorHandling(ret,"setsockopt1");
    
    //端口复用
    ret = setsockopt(servFd,SOL_SOCKET,SO_REUSEPORT,(const void *)&opt,sizeof(opt));
    errorHandling(ret,"setsockopt2");

    //初始化服务器sockaddr_in 
    //网络地址需要采用网络字节序存储(大端模式),x86平台的主机字节序是小端模式的
    struct sockaddr_in servAddr,clntAddr;
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8888);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //绑定地址到socket内核文件
    ret = bind(servFd,(struct sockaddr*)&servAddr,sizeof(servAddr));
    errorHandling(ret,"bind");
    
    //监听socket，此时内核文件的输入输出缓冲区变成半连接队列和全连接队列。
    ret = listen(servFd,128);   //第二个参数已经没有太多意义
    errorHandling(ret,"listen");
    printf("server is listening...\n");
    
    //创建epoll实例
    int efd = epoll_create1(0);     //红黑树+就绪链表

    //向内核注册就绪事件
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT; //监听读写事件
    ev.data.fd = servFd;
    ret = epoll_ctl(efd,EPOLL_CTL_ADD,servFd,&ev);
    if(ret < 0){
        perror("epoll ctl");
        close(servFd);      //回收文件描述符
        close(efd);
        exit(EXIT_FAILURE);
    }
    //struct epoll_event* evtList = new struct epoll_event;
    struct epoll_event* evtList = (struct epoll_event*)malloc(1024*sizeof(struct epoll_event));
    //事件循环
    while(1){
        //Reactor  反应器
        int nready = epoll_wait(efd,evtList,1024,3000);     //3000ms
        if(-1 == nready && errno == EINTR){//EINTR  The call was interrupted by a signal handler before either (1) any of the requested events occurred or (2) the timeout expired; see signal(7
            continue;
        }
        else if(-1 == nready){
            errorHandling(nready,"epoll_wait\n");
        }else if(0 == nready){  //超时
            printf(">>epoll_wait timeout 3000ms\n");
        }else{      //有就绪事件时
            //遍历struct epoll_event数组，去check
            //每一个epoll_event到底发生了什么
            for(int idx = 0; idx < nready; ++idx){
                //必须使用位运算&操作来判断事件，不可以用==，&&
                if((evtList[idx].data.fd == servFd) && 
                    (evtList[idx].events & EPOLLIN)){   //当监听文件描述符就绪并且epoll实例的事件为读就绪
                        //意味着有新连接来了，所以应该用accept函数，获取新连接
                        //写事件什么时候触发？只要内核发送缓存区还有空间，就可以触发写事件
                        socklen_t length = sizeof(clntAddr);
                        int clntFd = accept(servFd,(struct sockaddr*)&clntAddr,&length);
                        if(clntFd < 0){
                            perror("accept");
                            close(servFd);
                            exit(EXIT_FAILURE);
                        }

                        //将新的连接加入epoll 的实例中去
                        struct epoll_event newEv;
                        newEv.data.fd = clntFd;
                        newEv.events = EPOLLIN | EPOLLOUT | EPOLLERR;
                        ret = epoll_ctl(efd,EPOLL_CTL_ADD,clntFd,&newEv);
                        if(ret < 0){
                            perror("epoll ctl 2 failure\n"); 
                            continue;
                        }

                        //新连接到来后的处理
                        printf(">> conn has connected, fd: %d\n",clntFd);
                        //再就可以记录日志，使用Log4cpp完成
                        //个性化定制 ==》事件处理器
                        //onConnection()
                    }else{//已经建立好的连接发送数据过来了,此连接可能是任何客户端
                        //如果已经发生了读事件
                        char buff[128] = {0};
                        if(evtList[idx].events & EPOLLIN){
                            int fd = evtList[idx].data.fd;
                            ret = recv(fd,buff,sizeof(buff),0);
                            if(ret > 0){   //接受的数据大于1bytes
                                printf(">> recv msg %d bytes,content:%s\n",ret,buff);
                                
                                //1.对应用层协议解析。注意，上面的buff可能不够大，会影响性能
                                //2.拿到最终要处理的数据之后，进行业务逻辑处理
                                //注意：第二步中处理的时间太久也不行，服务的响应时间太长了
                                //3.得到想要的结果后，发送回客户
                                ret = send(fd,buff,sizeof(buff),0); //更多的应该是后端处理后的结果发送回去，发送buff就是一个echo服务了
                                printf(">> send %d bytes\n",ret);
                                //onMessage();  //考虑扩展性，挖坑
                            }else if(ret == 0){//接受0bytes
                                printf("conn has closed\n");

                                //需要从epoll实例中删除此连接对应的监听文件描述符
                                struct epoll_event ev;
                                ev.data.fd = fd;
                                ret = epoll_ctl(efd,EPOLL_CTL_DEL,fd,&ev);
                                if(ret < 0) 
                                {
								    perror("epoll_ctl");
							    }
                                //记录日志，log4cpp
                                //onclose()
                            }//end of ret condition
                        }//end of read event conditon
                    }//end of event conditon
            }//else if() //Reactor的其他情况
        }
    }
    close(servFd);
    return 0;
}

