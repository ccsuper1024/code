#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(8888);

    int ret = bind(listenFd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    ret = listen(listenFd, 128);

    int epfd = epoll_create(1);

    struct epoll_event evt1;
    evt1.data.fd = listenFd;
    evt1.events = EPOLLIN | EPOLLOUT;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenFd, &evt1);

    struct epoll_event *evtlist = (struct epoll_event *) malloc(sizeof(struct epoll_event) * 1024);

    while (true) {
        int ready = epoll_wait(epfd, evtlist, 1024, 5000);
        if (ready == -1 && errno == EINTR) {
            continue;
        } else if (-1 == ready) {
            perror("epoll wait\n");
            return -1;
        } else if (0 == ready) {
            printf("epoll_wait timeout\n");
        } else {
            for (int i = 0; i < 1024; ++i) {
                if (evtlist[i].data.fd == listenFd &&
                    (evtlist[i].events & EPOLLIN)) {    //读事件
                    int peerfd = accept(listenFd, nullptr, nullptr);

                    //将新的连接加入epoll实例中去
                    struct epoll_event evt2;
                    evt2.data.fd = peerfd;
                    evt2.events = EPOLLIN | EPOLLOUT;

                    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenFd, &evt2);
                    printf("new connection\n");
                    //三个半事件中的新连接的建立
                    //此处没有对新连接的消息进行处理
                } else {     //若是旧连接
                    //已经建立好的连接发送数据过来了
                    //如果是旧连接发生了读事件
                    char buff[128] = {0};
                    if (evtlist[i].events & EPOLLIN) {
                        int fd = evtlist[i].data.fd;
                        ret = ::recv(fd, buff, sizeof(buff), 0);
                        if (ret < 0) {
                            printf(">> recv msg %d bytes ,content: %s\n", ret, buff);

                            /*
                            1. 对应用层数据进行解析
                            2. 拿到最终要处理的数据之后，进行业务逻辑处理（假设这一步要很久是否合适？响应时间太长了）
                            3. 得到处理后的数据进行返回
                            */
                            ret = send(fd, buff, strlen(buff), 0);
                            printf(">> send %d bytes\n", ret);
                            //三个半事件中的消息送达
                        } else if (ret == 0) {
                            //recv的返回值为0，表示连接已经断开了
                            printf("connection has closed\n");

                            //将文件描述符从epoll实例中删除
                            struct epoll_event ev3;
                            ev3.data.fd = fd;
                            ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev3);
                            //三个半事件中的关闭连接
                        }
                    }// else if(evtlist[i].events & EPOLLOUT) {
//                    }
                }
            }
        }

    }
}
