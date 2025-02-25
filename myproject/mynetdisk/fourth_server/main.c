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

    MYSQL * mysql = mysql_init(NULL);
    MYSQL * ret = mysql_real_connect(mysql,"localhost","root","123","test",0,NULL,0);
    if(ret == NULL){
        fprintf(stderr,"error:%s\n", mysql_error(mysql));
        printf("数据库连接失败\n");
        exit(0);
    }

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
    int workerNum = atoi(num);
    threadPool_t threadPool;
    threadPool.tidArr = (pthread_t *)calloc(workerNum,sizeof(pthread_t));
    threadPool.workerNum = workerNum;
    pipe(threadPool.informPipe);
    close(threadPool.informPipe[1]);
    pthread_mutex_init(&threadPool.mutex,NULL);
    pthread_cond_init(&threadPool.cond,NULL);
    memset(&threadPool.taskQueue,0,sizeof(queue_t));
    threadPool.exitflag = 0;
    makeworker(&threadPool);

    //第四期所需变量
    int connectfd[1024]={0};
    user_t* userInfoArr[1024]={0};
    int connectIdx=0;

    int success = 1, filed = 0;
    char *salt =(char *) calloc(sizeof(char), 512);
    char *miwen = (char *)calloc(sizeof(char), 512);

    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);
    epollAdd(epfd,threadPool.informPipe[0]);
    epollAdd(epfd,exitPipe[0]);
    while(1){
        struct epoll_event readySet[2];
        int readyNum = epoll_wait(epfd,readySet,2,-1);
        for(int i = 0;i < readyNum;++i){
            if(readySet[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                connectfd[i]=netfd;
                userInfoArr[i]=(user_t*)calloc(1,sizeof(user_t));
                memcpy(userInfoArr[i]->rootPath,"/home/yangjunjie/home",24);
                connectIdx++;
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
            }else if(readySet[i].data.fd==threadPool.informPipe[0]){
                pthread_mutex_lock(&threadPool.mutex);
                epollAdd(epfd,threadPool.informfd);
                pthread_mutex_unlock(&threadPool.mutex);
            }
            else{
                for(int j=0;j<connectIdx;j++) {
                    if(connectfd[j]==readySet[i].data.fd){   
                        int flag = -1;
                        train_t train;
                        memset(&train,0,sizeof(train));
                        recv(connectfd[j],&train.length,sizeof(train.length),0);
                        recv(connectfd[j],train.data,train.length,0);
                        memcpy(&flag,train.data,sizeof(int));
                        printf("flag = %d\n",flag);


                        if(flag==0){//注册操作
                            printf("用户注册\n");
                            LOG_MESSAGE(LOG_INFO, "用户发起注册");
                            char name[50] = {0}, passwd[20] = {0};
                            memset(&train, 0, sizeof(train_t));
                            recv(connectfd[j], &train.length, sizeof(train.length), 0);
                            recv(connectfd[j], train.data, train.length, 0);
                            memcpy(name, train.data, train.length);
                            puts(name);
                            memset(&train, 0, sizeof(train_t));
                            recv(connectfd[j], &train.length, sizeof(train.length), 0);
                            recv(connectfd[j], train.data, train.length, 0);
                            memcpy(passwd, train.data, train.length);
                            puts(passwd);

                            // 先判断输入的用户名是否已经被占用
                            char sql[128] = {0};
                            sprintf(sql, "%s%s%s", "SELECT COUNT(*) FROM usertable WHERE username = '", name, "';");
                            int qret = mysql_query(mysql, sql);
                            if (qret != 0)
                            {
                                fprintf(stderr, "error 2:%s\n", mysql_error(mysql));
                                exit(0);
                            }
                            MYSQL_RES *res = mysql_store_result(mysql);
                            MYSQL_ROW row;
                            if((row = mysql_fetch_row(res)) != NULL){
                                printf("row[0] = %s\t", row[0]);
                            }else{
                                printf("row[0]取出不正确\n");
                            }

                            if (strcmp(row[0], "0") != 0)
                            { 
                                printf("名字已占用\n");
                                LOG_MESSAGE(LOG_INFO, "注册:名字被占用");
                                bzero(&train, sizeof(train));
                                train.length = sizeof(int);
                                memcpy(train.data, &filed, sizeof(int));
                                send(connectfd[j], &train, sizeof(train.length) + train.length, 0);

                                bzero(&train,sizeof(train));
                                train.length=strlen("yonghumingyicunzai");
                                memcpy(train.data,"yonghumingyicunzai",train.length);
                                send(connectfd[j],&train,sizeof(train.length)+train.length,0);
                                continue;
                            }

                            //到达此处说明用户名未被占用,下面将用户信息插入到数据库

                            GenerateStr(salt);
                            printf("salt: %s\n", salt);

                            miwen = crypt(passwd, salt);
                            bzero(sql,sizeof(sql));
                            sprintf(sql, "%s%s%s%s%s%s%s", "insert into usertable values (null,'", name, "','", salt, "','", miwen, "');");
                            qret = mysql_query(mysql, sql);
                            if (qret != 0)
                            {
                                fprintf(stderr, "error:%s\n", mysql_error(mysql));
                                exit(0);
                            }

                            printf("注册成功\n");
                            LOG_MESSAGE(LOG_INFO, "注册:注册成功");
                            bzero(&train, sizeof(train));
                            train.length = sizeof(int);
                            memcpy(train.data, &success, sizeof(int));
                            send(connectfd[j], &train, sizeof(train.length) + train.length, 0);

                            bzero(&train,sizeof(train));
                            train.length=strlen("zhucechenggong");
                            memcpy(train.data,"zhucechenggong",train.length);
                            send(connectfd[j],&train,sizeof(train.length)+train.length,0);
                            continue;
                        }
                        else if(flag == 1){  //登录
                            printf("用户登录\n");
                            LOG_MESSAGE(LOG_INFO, "用户发起登录");
                            char name[50] = {0}, passwd[20] = {0};
                            memset(&train, 0, sizeof(train_t));
                            recv(connectfd[j], &train.length, sizeof(train.length), 0);
                            recv(connectfd[j], train.data, train.length, 0);
                            memcpy(name, train.data, train.length);
                            puts(name);
                            memset(&train, 0, sizeof(train_t));
                            recv(connectfd[j], &train.length, sizeof(train.length), 0);
                            recv(connectfd[j], train.data, train.length, 0);
                            memcpy(passwd, train.data, train.length);
                            puts(passwd);

                            //先判断是否有该用户
                            char sql[128] = {0};
                            sprintf(sql, "%s%s%s", "SELECT COUNT(*) FROM usertable WHERE username = '", name, "';");
                            int qret = mysql_query(mysql, sql);
                            if (qret != 0)
                            {
                                fprintf(stderr, "error 2:%s\n", mysql_error(mysql));
                                exit(0);
                            }
                            MYSQL_RES *res = mysql_store_result(mysql);
                            MYSQL_ROW row;
                            if((row = mysql_fetch_row(res)) != NULL){
                                printf("row[0] = %s\t", row[0]);
                            }else{
                                printf("row[0]取出不正确\n");
                            }

                            if (strcmp(row[0], "0") == 0)
                            {
                                LOG_MESSAGE(LOG_INFO, "用户名不存在");
                                printf("用户名不存在\n");
                                bzero(&train, sizeof(train));
                                train.length = sizeof(int);
                                memcpy(train.data, &filed, sizeof(int));
                                send(connectfd[j], &train, sizeof(train.length) + train.length, 0);

                                bzero(&train,sizeof(train));
                                train.length=strlen("yonghumingbucunzai");
                                memcpy(train.data,"yonghumingbucunzai",train.length);
                                send(connectfd[j],&train,sizeof(train.length)+train.length,0);
                                continue;
                            }

                            //走到这里说明有该用户，需要取出盐值和密文
                            //SELECT salt FROM usertable WHERE username = name;
                            bzero(sql,sizeof(sql));
                            sprintf(sql, "%s%s%s", "SELECT salt FROM usertable WHERE username = '", name, "';");
                            qret = mysql_query(mysql, sql);
                            if (qret != 0)
                            {
                                fprintf(stderr, "error 2:%s\n", mysql_error(mysql));
                                exit(0);
                            }
                            res = mysql_store_result(mysql);

                            if((row = mysql_fetch_row(res)) != NULL){
                                printf("row[0] = %s\t", row[0]);
                            }else{
                                printf("row[0]取出不正确\n");
                            }

                            bzero(&salt,sizeof(salt));
                            salt=row[0];               //得到salt值


                            bzero(sql,sizeof(sql));   
                            sprintf(sql, "%s%s%s", "SELECT ciphertext FROM usertable WHERE username = '", name, "';");
                            qret = mysql_query(mysql, sql);
                            if (qret != 0)
                            {
                                fprintf(stderr, "error 2:%s\n", mysql_error(mysql));
                                exit(0);
                            }
                            res = mysql_store_result(mysql);
                            if((row = mysql_fetch_row(res)) != NULL){
                                printf("row[0] = %s\t", row[0]);
                            }else{
                                printf("row[0]取出不正确\n");
                            }
                            bzero(&miwen,sizeof(miwen));
                            miwen=row[0];                        //得到密文

                            if(strcmp(miwen,crypt(passwd,salt)) != 0){
                                LOG_MESSAGE(LOG_INFO, "密码错误");
                                printf("用户存在，密码错误\n");
                                bzero(&train, sizeof(train));
                                train.length = sizeof(int);
                                memcpy(train.data, &filed, sizeof(int));
                                send(connectfd[j], &train, sizeof(train.length) + train.length, 0);

                                bzero(&train,sizeof(train));
                                train.length=strlen("mimacuowu");
                                memcpy(train.data,"mimacuowu",train.length);
                                send(connectfd[j],&train,sizeof(train.length)+train.length,0);
                                continue;
                            }
                            else
                            {
                                LOG_MESSAGE(LOG_INFO, "登陆成功");
                                printf("登陆成功\n");
                                bzero(&train, sizeof(train));
                                train.length = sizeof(int);
                                memcpy(train.data, &success, sizeof(int));
                                send(connectfd[j], &train, sizeof(train.length) + train.length, 0);

                                bzero(&train,sizeof(train));
                                train.length=strlen("dengluchenggong");
                                memcpy(train.data,"dengluchenggong",train.length);
                                send(connectfd[j],&train,sizeof(train.length)+train.length,0);
                                bzero(sql,sizeof(sql));
                                sprintf(sql,"select * from usertable where userName='%s';",name);
                                mysql_query(mysql,sql);
                                MYSQL_RES* res=mysql_store_result(mysql);
                                MYSQL_ROW row=mysql_fetch_row(res);
                                userInfoArr[j]->userId=atoi(row[0]);
                                curDirInit(mysql,userInfoArr[j]);
                            }
                        }else if(flag == 2){
                            printf("flag==2\n");
                            int command;
                            char buff[1024] = {0};
                            recvCommand(connectfd[j],&command,buff,&train);
                            char buf[1024]={0};
                            int ret=0;
                            switch(command){
                            case 0:
                                //CD命令
                                printf("enter in cd\n");
                                strcpy(buf,"CD");
                                strcat(buf,buff);
                                LOG_MESSAGE(LOG_INFO,buf); 
                                ret=cdDir(mysql,userInfoArr[j],buff);
                                if(ret==-1){
                                    send(connectfd[j],"no such directory!",18,0);
                                    break;
                                }
                                pwd(mysql,userInfoArr[j],connectfd[j]);
                                break;
                            case 1:
                                //LS命令
                                strcpy(buf, "LS");
                                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                                list(*userInfoArr[j],mysql,buff,connectfd[j]);
                                break;
                            case 2:
                                //PUTS命令
                                strcpy(buf, "PUTS");
                                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));

                                pthread_mutex_lock(&threadPool.mutex);
                                Enqueue(&threadPool.taskQueue,connectfd[j],command,buff,userInfoArr[j]->curDirId);
                                printf("master send a task.\n");
                                pthread_cond_signal(&threadPool.cond);
                                pthread_mutex_unlock(&threadPool.mutex);
                                epollDel(epfd,connectfd[j]);
                                break;
                            case 3:
                                //GETS命令
                                strcpy(buf, "GETS");
                                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                                pthread_mutex_lock(&threadPool.mutex);
                                Enqueue(&threadPool.taskQueue,connectfd[j],command,buff,userInfoArr[j]->curDirId);
                                printf("master send a task.\n");
                                pthread_cond_signal(&threadPool.cond);
                                pthread_mutex_unlock(&threadPool.mutex);
                                epollDel(epfd,connectfd[j]);
                                break;
                            case 4:
                                //REMOVE命令
                                strcpy(buf, "REMOVE");
                                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                                removeThing(*userInfoArr[j],mysql,buff,connectfd[j]);
                                break;
                            case 5:
                                //PWD命令
                                printf("enter int pwd\n");
                                strcpy(buf, "PWD");
                                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                                pwd(mysql,userInfoArr[j],connectfd[j]);
                                break;
                            case 6:
                                //RMDIR命令
                                strcpy(buf, "RMDIR");
                                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                                removeThing(*userInfoArr[j],mysql,buff,connectfd[j]);
                                break;
                            case 7:
                                //MKDIR命令
                                strcpy(buf, "MKDIR");
                                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                                ret=mkDir(mysql,userInfoArr[j],buff);
                                if(ret==-1){
                                    send(connectfd[j],"error mkdir",11,0);
                                }
                                send(connectfd[j],"success",7,0);
                                break;
                            default:
                                LOG_MESSAGE(LOG_INFO, "invalid command");
                                send(connectfd[j],"invalid command",15,0);
                                break;
                            }
                        }else{
                            close(connectfd[j]);
                            LOG_MESSAGE(LOG_INFO, "用户断开连接");
                        }

                    }
                }
            }
        }}
    return 0;
}

