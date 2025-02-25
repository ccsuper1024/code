#include "head.h"

int main(int argc,char* argv[])
{
    // ./client 192.168.253.128 8080 file1
    ARGS_CHECK(argc,4);

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockfd,-1,"socket");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    int err = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    ERROR_CHECK(err,-1,"connect");

    printf("connet success!\n");

    clientRecvFile(sockfd,argv[3]); 

    close(sockfd);
    return 0;
}

