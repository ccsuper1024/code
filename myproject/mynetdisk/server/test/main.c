#include <func.h>
int main(int argc,char *argv[]){
    ARGS_CHECK(argc,2);
    int confFd = open(argv[1],O_RDONLY);
    ERROR_CHECK(confFd,-1,"open confFd");
    char buf[1024] = {0};
    //接受ip、port以及workerNUm
    char ip[20] = {0},port[20] = {0},num[20] = {0};
    read(confFd,buf,sizeof(buf));
    char* token = strtok(buf," ");
    memcpy(ip,token,strlen(token));
    token = strtok(NULL," ");
    memcpy(port,token,strlen(token));
    token = strtok(NULL," ");
    memcpy(num,token,strlen(token));
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    listen(sockfd,10);
    socklen_t len = sizeof(struct sockaddr);
    int netfd = accept(sockfd,NULL,&len);
    send(netfd,"hello",5,0);

    int curidx = 5;
    char buff[10] = "hello";
    char ch[4];
    sprintf(ch,"%d",curidx);
    strcat(buff,ch);
    puts(buff);
    return 0;
}
