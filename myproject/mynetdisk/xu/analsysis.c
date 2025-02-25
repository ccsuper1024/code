#include <func.h>
enum{
    CD,
    LS,
    PUTS,
    GETS,
    REMOVE,
    PWD,
    RMDIR,
    MKDIR
};


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
    printf("%s",token);

    token=strtok(NULL," ");
    if(token==NULL){
        return 0;
    }
    memcpy(path,token,strlen(token));

    if((token=strtok(NULL," "))!=NULL){
        return -1;
    }

    return 0;
}

int main()
{
    int cmd;
    char buf[]="mkdir wtfddf./././././../././././";
    char*path=(char*)calloc(100,sizeof(char));
    int ret=analysis(buf,&cmd,path);
    ERROR_CHECK(ret,-1,"analysis");
    printf("cmd=%d,path=%s",cmd,path);
    return 0;
}
