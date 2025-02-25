#include <func.h>
typedef struct train_s {
    int length;
    char data[1000];                                                      
} train_t;
int recvn(int sockfd,void *buf, int length,int localSize){
    int total = localSize;
    char *p = (char *)buf;
    while(total < length){
        ssize_t sret = recv(sockfd,p+total,length-total,0);
        if(sret < 0){
            break;
        }
        total += sret;
    }
    return 0;
}
int clientRecvFile(int sockfd){
    train_t train;
    bzero(&train,sizeof(train));
    recvn(sockfd,&train.length,sizeof(train.length),0);
    recvn(sockfd,train.data,train.length,0);
    int fd = open(train.data,O_RDWR|O_CREAT,0666);
    off_t filesize;
    recvn(sockfd,&train.length,sizeof(train.length),0);
    recvn(sockfd,train.data,train.length,0);
    memcpy(&filesize,train.data,sizeof(off_t));
    printf("filesize = %ld\n", filesize);

    //解决断点续传的问题
    struct stat statbuf;
    fstat(fd,&statbuf);
    printf("Local filesize = %ld\n",statbuf.st_size);
    //if(statbuf.st_size > 0){//只有断点续传时，需要进入这个逻辑
    //    lseek(fd,statbuf.st_size-2,SEEK_SET);
    //    char buf[256];
    //    bzero(buf,sizeof(buf));
    //    read(fd,buf,sizeof(buf));
    //    printf("buf = %s\n",buf);
    //    if(isprint(buf[0]) != 0 && isprint(buf[1]) == 0){//需要包含ctype.h
    //        printf("txt file!\n");
    //        lseek(fd,statbuf.st_size-1,SEEK_SET);
    //        int sizeToSend = statbuf.st_size - 1;
    //        send(sockfd,&sizeToSend,sizeof(int),MSG_NOSIGNAL);
    //    }
    //    else{
    //        printf("binary file!\n");
    //        lseek(fd,statbuf.st_size,SEEK_SET);
    //        send(sockfd,&statbuf.st_size,sizeof(statbuf.st_size),MSG_NOSIGNAL);
    //    }
    //}
    //else{
    //        lseek(fd,statbuf.st_size,SEEK_SET);
    //        send(sockfd,&statbuf.st_size,sizeof(statbuf.st_size),MSG_NOSIGNAL);
    //}
    lseek(fd,statbuf.st_size,SEEK_SET);
    //直接把本地已有的文件大小发过去，如果已有大小则是续传，根据文件大小进行偏移即可
    send(sockfd,&statbuf.st_size,sizeof(statbuf.st_size),MSG_NOSIGNAL);
    //
    //
    //
    //
    //
    //
    //
    ftruncate(fd,filesize);
    char *p = (char *)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,MAP_FAILED,"mmap");
    recvn(sockfd,p,filesize,statbuf.st_size);//statbuf.st_size是偏移量
    munmap(p,filesize);
    return 0;
}

