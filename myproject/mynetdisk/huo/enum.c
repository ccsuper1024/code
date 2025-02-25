#include"client.h"

int analysis(char* buf,int *cmd,char *path){
    char* token= strtok(buf," ");
    if(strcmp(token,"cd")==0){
        *cmd=CD;
    }
    else if(strcmp(token,"ls")==0){
        *cmd=LS;
    }
    else if(strcmp(token,"puts")==0){
        *cmd=PUTS;
    }
    else if(strcmp(token,"gets")==0){
        *cmd=GETS;
    }
    else if(strcmp(token,"remove")==0){
        *cmd=REMOVE;
    }
    else if(strcmp(token,"pwd")==0){
        *cmd=PWD;
    }
    else if(strcmp(token,"rmdir")==0){
        *cmd=RMDIR;
    }
    else if(strcmp(token,"mkdir")==0){
        *cmd=MKDIR;
    }else{
        return -1;
    }

    token=strtok(NULL," ");
    if(token==NULL){
        memcpy(path,"kong",4);
        return 0;
    }
    memcpy(path,token,strlen(token));

    if((token=strtok(NULL," "))!=NULL){   //3参数
        return -1;
    }

    return 0;
}

