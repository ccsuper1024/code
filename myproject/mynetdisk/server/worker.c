#include "threadPool.h"
#include "cjlhead.h"
void* threadFunc(void* arg){
    threadPool_t * pthreadPool = (threadPool_t *)arg;
    
    //线程一启动就连接数据库
    
    pthread_mutex_lock(&pthreadPool->mutex);
    MYSQL * mysql = mysql_init(NULL);
    MYSQL * ret = mysql_real_connect(mysql,"localhost","root","123","test",0,NULL,0);
    if(ret == NULL){
        fprintf(stderr,"error:%s\n", mysql_error(mysql));
        printf("数据库连接失败\n");
        exit(0);
    }
    pthread_mutex_unlock(&pthreadPool->mutex);
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
    /////////////////////////////////////////////////////////////
    //初始化
    user_t userInfo;
    bzero(userInfo.rootPath,sizeof(userInfo.rootPath));
    memcpy(userInfo.rootPath,"/home/yangjunjie/client",24);
    int success = 1, filed = 0;
    char *salt =(char *) calloc(sizeof(char), 512);
    char *miwen = (char *)calloc(sizeof(char), 512);


    while(1){
        printf("next while\n");
        //业务代码
        //初始化
        //接受标签，判断是否进行密码验证、执行命令或退出
        int flag = -1;
        train_t train;
        memset(&train,0,sizeof(train));
        recv(netfd,&train.length,sizeof(train.length),0);
        recv(netfd,train.data,train.length,0);
        memcpy(&flag,train.data,sizeof(int));
        printf("flag = %d\n",flag);


        if(flag==0){//注册操作
            printf("用户注册\n");
            LOG_MESSAGE(LOG_INFO, "用户发起注册");
            char name[50] = {0}, passwd[20] = {0};
            memset(&train, 0, sizeof(train_t));
            recv(netfd, &train.length, sizeof(train.length), 0);
            recv(netfd, train.data, train.length, 0);
            memcpy(name, train.data, train.length);
            puts(name);
            memset(&train, 0, sizeof(train_t));
            recv(netfd, &train.length, sizeof(train.length), 0);
            recv(netfd, train.data, train.length, 0);
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
            printf("total rows = %lu\n", mysql_num_rows(res));
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
                send(netfd, &train, sizeof(train.length) + train.length, 0);

                bzero(&train,sizeof(train));
                train.length=strlen("yonghumingyicunzai");
                memcpy(train.data,"yonghumingyicunzai",train.length);
                send(netfd,&train,sizeof(train.length)+train.length,0);
                continue;
            }

            //到达此处说明用户名未被占用,下面将用户信息插入到数据库
            bzero(salt,sizeof(salt));
            GenerateStr(salt);
            printf("salt: %s\n", salt);

            miwen = crypt(passwd, salt);
            bzero(sql,sizeof(sql));
            sprintf(sql, "%s%s%s%s%s%s%s", "insert into usertable values (null,'", name, "','", salt, "','", miwen, "');");
            qret = mysql_query(mysql, sql);

            printf("注册成功\n");
            LOG_MESSAGE(LOG_INFO, "注册:注册成功");
            bzero(&train, sizeof(train));
            train.length = sizeof(int);
            memcpy(train.data, &success, sizeof(int));
            send(netfd, &train, sizeof(train.length) + train.length, 0);

            bzero(&train,sizeof(train));
            train.length=strlen("zhucechenggong");
            memcpy(train.data,"zhucechenggong",train.length);
            send(netfd,&train,sizeof(train.length)+train.length,0);
            continue;
        }
        else if(flag == 1){  //登录
            printf("用户登录\n");
            LOG_MESSAGE(LOG_INFO, "用户发起登录");
            char name[50] = {0}, passwd[20] = {0};
            memset(&train, 0, sizeof(train_t));
            recv(netfd, &train.length, sizeof(train.length), 0);
            recv(netfd, train.data, train.length, 0);
            memcpy(name, train.data, train.length);
            puts(name);
            memset(&train, 0, sizeof(train_t));
            recv(netfd, &train.length, sizeof(train.length), 0);
            recv(netfd, train.data, train.length, 0);
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
            printf("total rows = %lu\n", mysql_num_rows(res));
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
                send(netfd, &train, sizeof(train.length) + train.length, 0);

                bzero(&train,sizeof(train));
                train.length=strlen("yonghumingbucunzai");
                memcpy(train.data,"yonghumingbucunzai",train.length);
                send(netfd,&train,sizeof(train.length)+train.length,0);
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
            printf("total rows = %lu\n", mysql_num_rows(res));

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
            printf("total rows = %lu\n", mysql_num_rows(res));
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
                send(netfd, &train, sizeof(train.length) + train.length, 0);

                bzero(&train,sizeof(train));
                train.length=strlen("mimacuowu");
                memcpy(train.data,"mimacuowu",train.length);
                send(netfd,&train,sizeof(train.length)+train.length,0);
                continue;
            }
            else
            {
                LOG_MESSAGE(LOG_INFO, "登陆成功");
                printf("登陆成功\n");
                bzero(&train, sizeof(train));
                train.length = sizeof(int);
                memcpy(train.data, &success, sizeof(int));
                send(netfd, &train, sizeof(train.length) + train.length, 0);

                bzero(&train,sizeof(train));
                train.length=strlen("dengluchenggong");
                memcpy(train.data,"dengluchenggong",train.length);
                send(netfd,&train,sizeof(train.length)+train.length,0);
                bzero(sql,sizeof(sql));
                sprintf(sql,"select * from usertable where userName='%s';",name);
                mysql_query(mysql,sql);
                MYSQL_RES* res=mysql_store_result(mysql);
                MYSQL_ROW row=mysql_fetch_row(res);
                userInfo.userId=atoi(row[0]);
                curDirInit(mysql,&userInfo);
            }
        }else if(flag == 2){
            printf("flag==2\n");
            int command;
            char buff[1024] = {0};
            recvCommand(netfd,&command,buff,&train);
            char buf[1024]={0};
            int ret=0;
            switch(command){
            case 0:
                //CD命令
                printf("enter in cd\n");
                strcpy(buf,"CD");
                strcat(buf,buff);
                LOG_MESSAGE(LOG_INFO,buf); 
                ret=cdDir(mysql,&userInfo,buff);
                if(ret==-1){
                    send(netfd,"no such directory!",18,0);
                    break;
                }
                pwd(mysql,&userInfo,netfd);
                break;
            case 1:
                //LS命令
                strcpy(buf, "LS");
                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                list(userInfo,mysql,buff,netfd);
                break;
            case 2:
                //PUTS命令
                strcpy(buf, "PUTS");
                ret = recvFile(netfd, buff, &userInfo, mysql);
                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                if(ret != 0)
                {
                    printf("PUTS ERROR!\n");
                }
                break;
            case 3:
                //GETS命令
                strcpy(buf, "GETS");
                ret = serverSendFile(netfd,buff,&userInfo,mysql);
                /* LOG_MESSAGE(LOG_INFO, strcat(buf, buff)); */
                break;
            case 4:
                //REMOVE命令
                strcpy(buf, "REMOVE");
                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                removeThing(userInfo,mysql,buff,netfd);
                break;
            case 5:
                //PWD命令
                printf("enter int pwd\n");
                strcpy(buf, "PWD");
                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                pwd(mysql,&userInfo,netfd);
                break;
            case 6:
                //RMDIR命令
                strcpy(buf, "RMDIR");
                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                removeThing(userInfo,mysql,buff,netfd);
                break;
            case 7:
                //MKDIR命令
                strcpy(buf, "MKDIR");
                LOG_MESSAGE(LOG_INFO, strcat(buf, buff));
                ret=mkDir(mysql,&userInfo,buff);
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
            LOG_MESSAGE(LOG_INFO, "用户断开连接");
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
