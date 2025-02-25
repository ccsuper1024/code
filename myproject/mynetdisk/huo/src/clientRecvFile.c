//#include "cjlhead.h"
#include "md5.h"
#include <func.h>
#include "client.h"
//typedef struct train_s{
//    int length;
//    char data[1024];
//}train_t;

int recvn(int sockfd,void* dst,int length){
    int total = 0;
    char* p = (char*)dst;
    while(total < length){
        ssize_t sret = recv(sockfd,p+total,length-total,0);
        ERROR_CHECK(sret,-1,"recv");
        total += sret;
        //printf("total = %d\n",total);
    }
    return 0;
}
int recv_write(int sockfd,int fdw,int length){//这里的length是要接收的size减去本地已有文件大小的值
    int total = 0;//这里total不用动，因为用的是write实现的写入文件，lseek已经实现了偏移
    char buf[4096];
    while(total < length){
        bzero(buf,sizeof(buf));
        //ssize_t sret = recv(sockfd,buf,length,0);
        //这里填length，如果收大文件length>>sizeof(buf),recv会直接报错
        ssize_t sret = recv(sockfd,buf,sizeof(buf),0);
        ERROR_CHECK(sret,-1,"recv");
        sret = write(fdw,buf,sret);
        ERROR_CHECK(sret,-1,"write");
        total += sret;
    }
    return 0;
}

int clientRecvFile(int sockfd,const char* filename)
{
    //打开文件，如无则创建
    int fd = open(filename,O_RDWR|O_CREAT,0666);
    ERROR_CHECK(fd,-1,"open");

    //先接收文件长度
    train_t train;
    //recv(sockfd,&train.length,sizeof(train.length),0);
    recvn(sockfd,&train.length,sizeof(train.length));
    off_t size;
    //recvn(sockfd,&size,train.length);
    recvn(sockfd,train.data,train.length);
    memcpy(&size,train.data,train.length);
    printf("filesize = %ld\n",size);

    //接收md5码
    bzero(&train,sizeof(train));
    recvn(sockfd,&train.length,sizeof(train.length));
    recvn(sockfd,train.data,train.length);
    //unsigned char md5[16] = {0};
    char md5[33] = {0};
    memcpy(md5,train.data,train.length);
    printf("md5:%s\n",md5);
    //for(int i = 0;i < 16;i++){
    //    printf("%02x",md5[i]);
    //}
    //printf("\n");

    //解决断点续传问题
    struct stat statbuf;
    fstat(fd,&statbuf);
    printf("Local size = %ld\n",statbuf.st_size);

    lseek(fd,statbuf.st_size,SEEK_SET);
    int err = send(sockfd,&statbuf.st_size,sizeof(statbuf.st_size),0);
    ERROR_CHECK(err,-1,"send");
    recv_write(sockfd,fd,size-statbuf.st_size);//注意这里size减去了已有的文件长度

    lseek(fd,0,SEEK_SET);
    unsigned char recv_file_md5[16] = {0};
    md5_function(fd,recv_file_md5);
    //MD5Final(&context,unsigned char digest[16])
    
    char xxx[33] = {0};
    char* p = xxx;
    for(int i = 0;i < 16;i++){
        sprintf(p,"%02x",recv_file_md5[i]);
        p += 2;
    }
    printf("md5:%s\n",xxx);
    
    int ret = strcmp(xxx,md5);
    if(ret == 0){
        printf("Download success!\n");
    }
    else{
        printf("Download failed!\n");
    }
    //int count = 0;
    //for(int i = 0;i < 16;i++){
    //    if(md5[i] == recv_file_md5[i]){
    //        count++;
    //    }
    //}
    //if(count == 16){
    //    printf("Download success!\n");
    //}
    //else{
    //    printf("Download failed!\n");
    //}
    //for(int i = 0;i < 16;i++){
    //    printf("%02x",md5[i]);
    //}
    //printf("\n");

    close(fd);
    return 0;
}

    //ftruncate(fd,size);

    //char* p = (char*)mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ////recv_write(sockfd,fd,size);
    //recvn(sockfd,p,size);
    //munmap(p,size);
