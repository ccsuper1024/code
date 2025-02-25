#include "head.h"
typedef struct train_s{
    int length;
    char data[1024];
}train_t;


int find_md5(const char* filename,user_t* puser,char* ret,MYSQL* mysql){//根据filename和curDirId可以唯一确定一个文件
    //查表找到md5码，32位的字符串
    //char buf[] = "select md5str from filetable where pre_id = '1' and name = 'file1';";
    char buf[256];
    bzero(buf,sizeof(buf));
    sprintf(buf,"select md5str from filetable where pre_id = '%d' and name = '%s';",puser->curDirId,filename);
    int qret =  mysql_query(mysql,buf);                                                                                                 
    if(qret != 0){
        fprintf(stderr,"error:%s\n",mysql_error(mysql));
        exit(1);
    }
    MYSQL_RES* res = mysql_store_result(mysql);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    printf("md5:%s\n",row[0]);

    memcpy(ret,row[0],32);
    return 0;
}  

//filename还是那个虚拟文件名，不是md5码
int serverSendFile(int netfd,const char* filename,user_t* puser,MYSQL* mysql)
{
    //查表找到md5码，md5码就是文件名，用文件名open
    char buf[256];
    bzero(buf,sizeof(buf));
    memcpy(buf,puser->rootPath,strlen(puser->rootPath)); 
    char ret_tmp[33];
    bzero(ret_tmp,sizeof(ret_tmp));

    find_md5(filename,puser,ret_tmp,mysql);
    printf("md5:%s\n",ret_tmp);
    strcat(buf,"/");
    strcat(buf,ret_tmp);

    int fd = open(buf,O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    struct stat statbuf;
    int err = fstat(fd,&statbuf);
    ERROR_CHECK(err,-1,"fstat");

    //将文件的长度发过去
    train_t train;
    train.length = sizeof(statbuf.st_size);//off_t类型占8个字节
    memcpy(train.data,&statbuf.st_size,train.length);
    printf("filesize = %ld\n",statbuf.st_size);
    err = send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    ERROR_CHECK(err,-1,"send length");

    //把文件的MD5码发过去
    //unsigned char md5[16] = {0};
    //md5_function(fd,md5);
    ////for(int i = 0;i < 16;i++){
    ////    printf("%02x",md5[i]);
    ////}
    //char test[33] = {0};
    //char* p = test;
    //for(int i = 0;i < 16;i++){
    //    sprintf(p,"%02x",md5[i]);
    //    p += 2;
    //}
    //printf("md5:%s\n",test);
    //printf("\n");

    train.length = sizeof(ret_tmp);//md5码是一个32位的字符串,train.length的值为33
    bzero(train.data,sizeof(train.data));
    memcpy(train.data,ret_tmp,train.length);
    err = send(netfd,&train,sizeof(train.length)+train.length,MSG_NOSIGNAL);
    ERROR_CHECK(err,-1,"send");


    //接收客户端的信息，用于lseek偏移，实现断点续传
    off_t offset;
    err = recv(netfd,&offset,sizeof(offset),MSG_WAITALL);
    ERROR_CHECK(err,-1,"recv");

    lseek(fd,offset,SEEK_SET);

    //一次性发送整个文件
    sendfile(netfd,fd,0,statbuf.st_size-offset);
    return 0;
}

