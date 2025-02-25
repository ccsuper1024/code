#include "cjlhead.h"

int md5_function(int fd,unsigned char* ret){
    MD5_CTX context;
    MD5Init(&context);//使用MD5结构一定要初始化，否则结果出错
    struct stat statbuf;
    int err = fstat(fd,&statbuf);
    ERROR_CHECK(err,-1,"fstat");
    //printf("size = %ld\n",statbuf.st_size);
    unsigned char* data = (unsigned char*)malloc(statbuf.st_size);
    ERROR_CHECK(data,NULL,"malloc");

    ssize_t sret = read(fd,data,statbuf.st_size);
    ERROR_CHECK(sret,-1,"sret");

    MD5Update(&context, data, statbuf.st_size);

    unsigned char dig[16] = {0};

    MD5Final(&context, ret);

    //for(int i = 0;i < 16;i++){
    //    printf("%02x",dig[i]);
    //}
    //printf("\n");
    free(data);
    return 0;
}
