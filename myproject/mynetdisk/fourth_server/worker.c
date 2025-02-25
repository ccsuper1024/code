#include "threadPool.h"
#include "cjlhead.h"
void* threadFunc(void* arg){
    threadPool_t * pthreadPool = (threadPool_t *)arg;

    //线程一启动就连接数据库

    pthread_mutex_lock(&pthreadPool->mutex);
    close(pthreadPool->informPipe[0]);
    MYSQL * mysql = mysql_init(NULL);
    MYSQL * ret = mysql_real_connect(mysql,"localhost","root","123","test",0,NULL,0);
    if(ret == NULL){
        fprintf(stderr,"error:%s\n", mysql_error(mysql));
        printf("数据库连接失败\n");
        exit(0);
    }
    pthread_mutex_unlock(&pthreadPool->mutex);

    while(1){ 
        pthread_mutex_lock(&pthreadPool->mutex);
        while(pthreadPool->taskQueue.queueSize == 0 && pthreadPool->exitflag == 0){
            pthread_cond_wait(&pthreadPool->cond,&pthreadPool->mutex);
        }
        if(pthreadPool->exitflag == 1){
            pthread_mutex_unlock(&pthreadPool->mutex);
            pthread_exit(NULL);
        }
        printf("a worker got a task!\n");
        int netfd = pthreadPool->taskQueue.pfront->fd;
        int flag=pthreadPool->taskQueue.pfront->flag;
        int curId=pthreadPool->taskQueue.pfront->dirId;
        char cmdStr[512]={0};
        memcpy(cmdStr,pthreadPool->taskQueue.pfront->cmdStr,strlen(pthreadPool->taskQueue.pfront->cmdStr));    
        Dequeue(&pthreadPool->taskQueue);
        pthread_mutex_unlock(&pthreadPool->mutex);
        //puts命令
        user_t userInfo;
        userInfo.curDirId=curId;
        memcpy(userInfo.rootPath,"/home/yangjunjie/client",24);
        if(flag==2){
            int ret=recvFile(netfd,cmdStr,&userInfo,mysql);
            if(ret==-1){
                puts("puts error");
            }
            pthread_mutex_lock(&pthreadPool->mutex);
            pthreadPool->informfd=netfd;
            write(pthreadPool->informPipe[1],"1",1);
            pthread_mutex_unlock(&pthreadPool->mutex);
        }else{
            int ret=serverSendFile(netfd,cmdStr,&userInfo,mysql);
            if(ret==-1){
                puts("gets error");
        }
        pthread_mutex_lock(&pthreadPool->mutex);
        pthreadPool->informfd=netfd;
        write(pthreadPool->informPipe[1],"1",1);
        pthread_mutex_unlock(&pthreadPool->mutex);
        }
    }

    return 0;
}

int makeworker(threadPool_t *pthreadPool){
    for(int i = 0;i < pthreadPool->workerNum;++i){
        pthread_create(&pthreadPool->tidArr[i],NULL,threadFunc,pthreadPool);
    }
    return 0;
}
