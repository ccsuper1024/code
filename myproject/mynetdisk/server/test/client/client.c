#include <func.h>
int main(int argc,char *argv[]){
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    char buf[1024] = {0};
    recv(sockfd,buf,sizeof(buf),0);
    puts(buf);
    close(sockfd);
    return 0;
}
