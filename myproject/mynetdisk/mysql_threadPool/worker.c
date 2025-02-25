#include "threadPool.h"
void* threadFunc(void* arg){
    threadPool_t * pthreadPool = (threadPool_t *)arg;

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
    printf("netfd = %d\n",netfd);
    Dequeue(&pthreadPool->taskQueue);
    pthread_mutex_unlock(&pthreadPool->mutex);
    //初始化路径和栈
    path_t *curPath = (path_t*)calloc(1,sizeof(path_t));
    curPath->rootPath=(char*)calloc(40,1);
    memcpy(curPath->rootPath,"/home/yangjunjie/client",24);
    curPath->userPath=(char*)calloc(200,1);
    curPath->userPath[0]='/';
    char ch[4] = {0};
    sprintf(ch,"%d",curidx);
    puts(ch);
    char buffer[1024] = {0};
    sprintf(buffer,"%s%s",curPath->rootPath,ch);
    memcpy(curPath->rootPath,buffer,strlen(buffer)+4);
    Stack_t* s=stackInit();
    char *filename = (char *)calloc(1024,sizeof(char));
    printf("root path = %s\n",curPath->rootPath);
    while(1){
        printf("next while\n");
        //业务代码
        //初始化
        //接受标签，判断是否进行密码验证、执行命令或退出
        int flag = 0;
        train_t train;
        memset(&train,0,sizeof(train));
        recv(netfd,&train.length,sizeof(train.length),0);
        recv(netfd,train.data,train.length,0);
        memcpy(&flag,train.data,sizeof(int));
        printf("flag = %d\n",flag);
        if(flag == 1){
            //密码验证
            LOG_MESSAGE(LOG_INFO, "接入");
            char name[50] = {0},passwd[20] = {0};
            memset(&train,0,sizeof(train_t));
            recv(netfd,&train.length,sizeof(train.length),0);
            recv(netfd,train.data,train.length,0);
            memcpy(name,train.data,train.length);
            puts(name);
            memset(&train,0,sizeof(train_t));
            recv(netfd,&train.length,sizeof(train.length),0);
            recv(netfd,train.data,train.length,0);
            memcpy(passwd,train.data,train.length);
            puts(passwd);
            int ret = passwdCrypted(name,passwd);
            printf("ret = %d\n",ret);
            if(ret == -1){
                send(netfd,"Enter username and passeword again.\n",34,0); 
                close(netfd); 
            }else{ 
                curidx++; 
                printf("curidx = %d\n",curidx); 
                LOG_MESSAGE(LOG_INFO, "密码验证成功"); 
            } 
        }else if(flag == 2){
            printf("flag==2\n");
            int command;
            char buff[1024] = {0};
            recvCommand(netfd,&command,buff,&train);
            bzero(filename,1024);
            sprintf(filename,"%s%s%s",curPath->rootPath,curPath->userPath,buff);
            printf("command = %d\n",command);
            printf("filename=%s\n",filename);
            char buf[1024]={0};
            int ret=0;
            switch(command){
            case 0:
                //CD命令
                printf("enter in cd\n");
                strcpy(buf,"CD");
                LOG_MESSAGE(LOG_INFO,strcat(buf,filename));
                ret=cd(s,curPath,buff);
                printf("userPath=%s\n",curPath->userPath);
                for(int i=0;i<s->top;i++){
                    printf("i=%d,stack=%s",i,s->elements[i]);
                }
                printf("end\n");
                if(ret==-1){
                    send(netfd,"no such directory!",18,0);
                }
                pwd(netfd,curPath);
                break;
            case 1:
                //LS命令
                strcpy(buf, "LS");
                LOG_MESSAGE(LOG_INFO, strcat(buf, filename));
                list(*curPath,buff,netfd);
                break;
            case 2:
                //PUTS命令
                strcpy(buf, "PUTS");
                LOG_MESSAGE(LOG_INFO, strcat(buf, filename));
                recvFile(netfd,filename);
                break;
            case 3:
                //GETS命令
                strcpy(buf, "GETS");
                LOG_MESSAGE(LOG_INFO, strcat(buf, filename));
                serverSendFile(netfd,filename);
                break;
            case 4:
                //REMOVE命令
                strcpy(buf, "REMOVE");
                LOG_MESSAGE(LOG_INFO, strcat(buf, filename));
                removeThing(*curPath,buff,netfd);
                break;
            case 5:
                //PWD命令
                printf("enter int pwd\n");
                strcpy(buf, "PWD");
                LOG_MESSAGE(LOG_INFO, strcat(buf, filename));
                pwd(netfd,curPath);
                break;
            case 6:
                //RMDIR命令
                strcpy(buf, "RMDIR");
                LOG_MESSAGE(LOG_INFO, strcat(buf, filename));
                break;
            case 7:
                //MKDIR命令
                strcpy(buf, "MKDIR");
                LOG_MESSAGE(LOG_INFO, strcat(buf, filename));
                ret=mkDir(filename);
                if(ret==-1){
                    send(netfd,"error mkdir",11,0);
                }
                send(netfd,"success",7,0);
                break;
            default:
                LOG_MESSAGE(LOG_INFO, "invalid command");
                send(netfd,"invalid command",15,0);
                break;
            }
        }else{
            close(netfd);
            while(pthreadPool->taskQueue.queueSize == 0 && pthreadPool->exitflag == 0){
                pthread_cond_wait(&pthreadPool->cond,&pthreadPool->mutex);
            }
            if(pthreadPool->exitflag == 1){
                pthread_mutex_unlock(&pthreadPool->mutex);
                pthread_exit(NULL);
            }
            printf("a worker got a task!\n");
            int netfd = pthreadPool->taskQueue.pfront->fd;
            printf("netfd = %d\n",netfd);
            Dequeue(&pthreadPool->taskQueue);
            pthread_mutex_unlock(&pthreadPool->mutex);
        }
    }
}


int makeworker(threadPool_t *pthreadPool){
    for(int i = 0;i < pthreadPool->workerNum;++i){
        pthread_create(&pthreadPool->tidArr[i],NULL,threadFunc,pthreadPool);
    }
    return 0;
}
