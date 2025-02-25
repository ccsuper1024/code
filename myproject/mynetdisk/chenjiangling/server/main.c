#include "head.h"


int main(int argc,char* argv[])
{
    // ./server 0.0.0.0 8080 file1
    int err;
    ARGS_CHECK(argc,4);

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    err = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    ERROR_CHECK(err,-1,"bind");

    listen(sockfd,10);

    printf("listen success!\n");

    char buf[256];
    bzero(buf,sizeof(buf));

    struct sockaddr_in client_addr;
    socklen_t client_addr_length = sizeof(client_addr);
    int netfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_addr_length);
    ERROR_CHECK(netfd,-1,"accept");
    printf("client ip :%s,client port :%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

    //建立与数据库的连接
    MYSQL* mysql = mysql_init(NULL);
    MYSQL* ret = mysql_real_connect(mysql,"localhost","root","920608","cloud_project",0,NULL,0);
    if(ret == NULL){
        fprintf(stderr,"error:%s\n",mysql_error(mysql));
        exit(1);
    }

    user_t user = {1,1,"/home/chenyi/Linux/project_test/server/test_file"};
    serverSendFile(netfd,argv[3],&user,mysql);
    mysql_close(mysql);
    close(netfd);
    close(sockfd);
    return 0;
}

