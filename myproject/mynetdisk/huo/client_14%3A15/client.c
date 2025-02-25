#include "client.h"
int main(int argc, char *argv[])
{
    //LOG_MESSAGE(LOG_INFO, "message");
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    // argv[1] "192.168.118.128" --> 32bit的大端ip地址
    int ret = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "connect");
    printf("connected!\n");
    train_t train;
    char buf[4096] = {0};
    int cmd;
    char path[128];

    //标签 
    //1 用户名、密码
    //2 命令
    //3 退出
    int k=0,a=1,b=2,c=3;
while(1){
    printf("注册扣0，登录扣1\n");
    int xuan=0;
    scanf("%d",&xuan);
    //发送注册还是登录的标签
    bzero(&train,sizeof(train));
    train.length=sizeof(train.length);
    memcpy(train.data,&xuan,train.length);
    send(sockfd,&train,sizeof(train.length)+train.length,0);

    printf("请输入用户名与密码\n");
    
    printf("用户名: \n");
    // bzero(&train,sizeof(train));
    // train.length=sizeof(int);
    // memcpy(train.data,&a,sizeof(int));
    // send(sockfd,&train,sizeof(train.length)+train.length,0);
    
    scanf(" %[^\n]", buf);
    printf("buf = %s\n",buf);
    bzero(&train,sizeof(train));
    train.length=strlen(buf);
    memcpy(train.data,buf,strlen(buf));
    send(sockfd,&train,sizeof(train.length)+train.length,0);
    //send密码
    printf("密码：\n");
    bzero(buf,sizeof(buf));
    scanf(" %[^\n]", buf);
    bzero(&train,sizeof(train));
    train.length=strlen(buf);
    memcpy(train.data,buf,strlen(buf));
    send(sockfd,&train,sizeof(train.length)+train.length,0);
    
    //接收数据  注册可能情况: 1.用户名被占用 2.注册成功
    //         登录可能情况： 1.用户名不存在 2.密码错误 3.登陆成功
    bzero(&train,sizeof(train));
    recv(sockfd,&train.length,sizeof(int),0);
    recv(sockfd,train.data,train.length,0);
    memcpy(&k,train.data,train.length);     //k=0,失败   k=1成功
    printf("登录接收1:%d\n",k);

    bzero(&train,sizeof(train));
    bzero(buf,sizeof(buf));
    recv(sockfd,&train.length,sizeof(int),0);

    printf("train.length = %d\n",train.length);

    recv(sockfd,buf,train.length,0);
    printf("登录接收2:%s\n",buf);
    if(k==0){  //失败
        printf("%s\n",buf);
        printf("请重新选择\n");
        continue;
    }else{ //成功
        printf("%s\n",buf);
        break;
    }  
}
    printf("\n");
    while (1)
    {
        printf("下次循环开始\n");
        bzero(buf,sizeof(buf));
        scanf(" %[^\n]", buf);
        printf("buf = %s,strlen = %ld \n",buf,strlen(buf));
        if (strcmp(buf, "quit") == 0)
        {
            bzero(&train,sizeof(train));
            train.length=sizeof(int);
            memcpy(train.data,&c,sizeof(int));
            send(sockfd,&train,sizeof(train.length)+train.length,0);
            send(sockfd,&train,sizeof(train.length)+train.length,0);
            send(sockfd,&train,sizeof(train.length)+train.length,0);
            break;
        }
        // 对命令切片
        bzero(path,sizeof(path));
        int ret = analysis(buf, &cmd, path);
        printf("cmd = %d ,path = %s,ret = %d\n ",cmd,path,ret);
        
        if (ret == -1)
        {
            printf("analysis error!\n");
            continue;
        }

        // 发送int型的enum，再发送char类型的参数
        bzero(&train,sizeof(train));
        train.length=sizeof(int);
        memcpy(train.data,&b,sizeof(int));
        send(sockfd,&train,sizeof(train.length)+train.length,0);

        bzero(&train,sizeof(train));
        train.length = sizeof(int);
        memcpy(train.data, &cmd, sizeof(int));
        send(sockfd, &train, sizeof(train.length) + train.length, MSG_NOSIGNAL);
        
        if(strcmp(path,"kong")==0)
        {
            bzero(&train, sizeof(train));
            //train.length = 0;
            //memcpy(train.data, &k, strlen(path));
            send(sockfd, &train, sizeof(train.length), MSG_NOSIGNAL);
            printf("已发送零字节\n");
        }else{
        bzero(&train,sizeof(train));
        train.length = strlen(path);
        memcpy(train.data, path, strlen(path));
        send(sockfd, &train, sizeof(train.length) + train.length, MSG_NOSIGNAL);
        }
        // 根据切片执行不同操作
        switch (cmd)
        {
        case CD:
        case LS:
        case REMOVE:
        case PWD:
        case RMDIR:
        case MKDIR:
            // 这几个命令需要接收 先接收，并将接收到的字符串打印，与目录有关
            //while (1)
            //{
                bzero(buf, sizeof(buf));
                int sret = recv(sockfd, buf, sizeof(buf), 0);
                if (sret == 0)
                {
                    break;
                }
                printf("%s\n", buf);
            //}
            break;
        case GETS:
            printf("downing!\n");
            clientRecvFile(sockfd,path);
            break;
        case PUTS:
            // 应该要在客户端调用puts函数
            upLoad(sockfd,path);
            break;
        }
        printf("本次循环结束\n");
    }
    
    close(sockfd);
}
