#include <func.h>
typedef struct train_s {
    int length;
    char data[1000];
} train_t;
int serverSendFile(int netfd,const char* filename){
    train_t train;
    //train.length = strlen(filename);
    //bzero(train.data,sizeof(train.data));
    //strcpy(train.data,filename);

    //send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    int fd = open(filename,O_RDWR);
    ERROR_CHECK(fd, -1, "oopen");
    struct stat statbuf;
    int ret = fstat(fd,&statbuf);
    ERROR_CHECK(ret, -1, "fstat");
    printf("filesize = %ld\n", statbuf.st_size);
    train.length = sizeof(statbuf.st_size);
    memcpy(train.data,&statbuf.st_size,train.length);
    send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    //处理断点续传
    //int offset;
    //ssize_t sret = recv(netfd,&offset,sizeof(int),0);
    //ERROR_CHECK(sret,-1,"recv");
    //int err = lseek(fd,offset,SEEK_SET);
    //ERROR_CHECK(err,-1,"lseek");
    ////////////////////////////////////////////
    //sendfile(netfd,fd,NULL,statbuf.st_size-offset);
    sendfile(netfd,fd,NULL,statbuf.st_size);
    
    //sleep(10);
    printf("sendFile success\n");
    return 0;
}
