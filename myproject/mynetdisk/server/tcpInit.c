#include "threadPool.h"
int tcpInit(const char* ip,const char* port,int *psockfd){
    //创建socket文件    
    *psockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(*psockfd,-1,"socket");
    //服务端地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    //地址复用
    int reuse = 1;
    int ret = setsockopt(*psockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");
    //端口复用
    int reuse2 = 1;
    ret = setsockopt(*psockfd,SOL_SOCKET,SO_REUSEPORT,&reuse2,sizeof(reuse1));
    ERROR_CHECK(ret,-1,"setsockopt");

    ret = bind(*psockfd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");

    listen(*psockfd,10);

    return 0;
}
