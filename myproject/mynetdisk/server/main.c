#include "threadPool.h"
int exitPipe[2];
void sigFunc(int signum){
    printf("parent is going to exit\n");
    write(exitPipe[1],"1",1);
}
int main(int argc,char* argv[]){
    ARGS_CHECK(argc,2);
    pipe(exitPipe);
    if(fork() != 0){
        close(exitPipe[0]);
        signal(SIGUSR1,sigFunc);
        wait(NULL);
        exit(0);
    }
    close(exitPipe[1]);
    int confFd = open(argv[1],O_RDONLY);
    ERROR_CHECK(confFd,-1,"open confFd");
    char buf[1024] = {0};
    //接受ip、port以及workerNum
    char ip[13] = {0},port[5] = {0},num[2] = {0};
    read(confFd,buf,sizeof(buf));
    char* token = strtok(buf," ");
    memcpy(ip,token,strlen(token));
    token = strtok(NULL," ");
    memcpy(port,token,strlen(token));
    token = strtok(NULL," ");
    memcpy(num,token,strlen(token));
    int sockfd;
    tcpInit(ip,port,&sockfd);
    //初始化全局变量
    curidx = 0;
    int workerNum = atoi(num);
    threadPool_t threadPool;
    threadPool.tidArr = (pthread_t *)calloc(workerNum,sizeof(pthread_t));
    threadPool.workerNum = workerNum;
    pthread_mutex_init(&threadPool.mutex,NULL);
    pthread_cond_init(&threadPool.cond,NULL);
    memset(&threadPool.taskQueue,0,sizeof(queue_t));
    threadPool.exitflag = 0;
    makeworker(&threadPool);

    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);
    epollAdd(epfd,exitPipe[0]);
    while(1){
        struct epoll_event readySet[2];
        int readyNum = epoll_wait(epfd,readySet,2,-1);
        for(int i = 0;i < readyNum;++i){
            if(readySet[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                pthread_mutex_lock(&threadPool.mutex);
                Enqueue(&threadPool.taskQueue,netfd);
                printf("master send a task.\n");
                pthread_cond_signal(&threadPool.cond);
                pthread_mutex_unlock(&threadPool.mutex);
            }else if(readySet[i].data.fd == exitPipe[0]){
                printf("threadPool is going to exit.\n");
                pthread_mutex_lock(&threadPool.mutex);
                threadPool.exitflag = 1;
                pthread_cond_broadcast(&threadPool.cond);
                pthread_mutex_unlock(&threadPool.mutex);
                for(int j = 0;j < workerNum;++j){
                    pthread_join(threadPool.tidArr[j],NULL);
                }
                printf("all worker is exited.\n");
                pthread_exit(NULL);
            }
        }
    }
    return 0;
}
