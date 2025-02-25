#include "threadPool.h"
int recvCommand(int netfd,int *command,char *buff,train_t *train){

    memset(train,0,sizeof(train_t));
    recv(netfd,&train->length,sizeof(train->length),0);
    recv(netfd,train->data,train->length,0);
    memcpy(command,train->data,sizeof(train->length));
    recv(netfd,&train->length,sizeof(train->length),0);
    if(train->length==0){
        bzero(buff,strlen(buff));
        return 0;
    }
    recv(netfd,train->data,train->length,0);
    memcpy(buff,train->data,train->length);
    return 0;
}
