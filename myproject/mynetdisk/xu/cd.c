#include <func.h>
#include "cd.h"


void visitStack(Stack_t* s){
    for(int i=0;i<s->top;i++){
        printf("i=%d,ele=%s ",i,s->elements[i]);
    }
    printf("\n");
}

Stack_t* stackInit(){
    Stack_t* stack=(Stack_t*)calloc(1,sizeof(Stack_t));
    for(int i=0;i<STACKLENTH;i++){
    stack->elements[i]=(char*)calloc(40,1);
    }
    return stack;
}

int stackDestory(Stack_t* stack){
    for(int i=0;i<STACKLENTH;i++){
        free(stack->elements[i]);
    }
    free(stack);
    return 0;
}

//简化路径 如/w/../a/./../b/c化简为/b/c,并重新赋值给path
int simplifyPath(Stack_t * s,char * path){
    char* str=strtok(path,"/");
    while(str){
        if(strcmp(str,"..")==0){
            if(s->top!=0){
            bzero(s->elements[s->top-1],strlen(s->elements[s->top-1]));
            s->top--;
            }
        }else if(strcmp(str,".")==0){
        }else{
            strncpy(s->elements[s->top],str,strlen(str)+1);
            s->top++;
        }
        str=strtok(NULL,"/");
    }
    char* res=(char*)calloc(512,1);

    for(int i=0;i<s->top;i++){
        strcat(res,"/");
        strcat(res,s->elements[i]);
    }
    strcat(res,"/");
    memcpy(path,res,strlen(res)+1);
    free(res);
    return 0;
}

//生成绝对路径
int generatePath(char* absolutePath, char* rootPath,char* path){
    memcpy(absolutePath,rootPath,strlen(rootPath)+1);
    strcat(absolutePath,path);
    return 0;
}

//会改变结构体里的path，若cd的目录不存在或是文件返回-1并且不会修改userpath
int cd(Stack_t* stack,path_t* curPath,char* str){
    if(str[0]=='\0'){
        return 0;
    }
    char absolutePath[256]={0};
    
    //cd 若后面的是绝对路径
    if(str[0]=='/'){
        generatePath(absolutePath,curPath->rootPath,str);//生成路径去打开目录尝试
        printf("openPath=%s\n",absolutePath);
        DIR* fddir=opendir(absolutePath);
        if(fddir==NULL){
            printf("NO such direcotory\n");
            closedir(fddir);
            return -1;//测试请注释掉,并去掉下方else结构
        }
        for(int i=0;i<stack->top;i++){
            bzero(stack->elements[i],20);
        }
        stack->top=0;
        simplifyPath(stack,str);//在临时栈里进行操作,若目标文件打不开不影响正常维护的栈
        memcpy(curPath->userPath,str,strlen(str)+1);//给结构体中的客户端路径
        closedir(fddir);
    }else{//基于原来的栈进行操作

        //看是否有该目录
        generatePath(absolutePath,curPath->rootPath,curPath->userPath);
        strcat(absolutePath,str);
        printf("openPath=%s\n",absolutePath);
        DIR* fddir=opendir(absolutePath);
        if(fddir==NULL){
            printf("NO such direcotory\n");
            closedir(fddir);
            return -1;
        }
        closedir(fddir);
        simplifyPath(stack,str);        
        memcpy(curPath->userPath,str,strlen(str)+1);//将所得字符串放入userpath中

    }

    return 0;
}

//获得userpath并且处理了发送行为
int pwd(int netfd,const path_t* curPath){
    if(curPath->userPath[0]=='\0'){
        send(netfd,"/",2,MSG_NOSIGNAL);
    }else{
        char buf[256]={0};
        memcpy(buf,curPath->userPath,strlen(curPath->userPath)+1);
        send(netfd,buf,sizeof(buf),MSG_NOSIGNAL);
    }
    return 0;
}

int mkDir(path_t* curPath){
    char path[256]={0};
    generatePath(path,curPath->rootPath,curPath->userPath);
    int ret=mkdir(path,0777);
    return ret;
}

int main(){
    path_t* curPath=(path_t *)calloc(1,sizeof(path_t));
    curPath->userPath=(char*)calloc(200,1);
    curPath->userPath[0]='/';
    Stack_t* s=stackInit();
    curPath->rootPath=(char*)calloc(20,1);
    memcpy(curPath->rootPath,"/home/douglas",13);
    /* curPath->rootPath="/home/douglas"; */
    /* int sret=simplifyPath(s,argv[1]); */
    /* ERROR_CHECK(sret,-1,"simplifyPath"); */
    /* printf("path=%s\n",path); */
    char buf[256]={0};
    while(1){
        bzero(buf,sizeof(buf));
        scanf(" %s",buf);
        printf("buf=%s\n",buf);
        cd(s,curPath,buf);
        printf("rootPath=%s\n",curPath->rootPath);
        printf("userpath=%s\n",curPath->userPath);
        char absolutePath[256]={0};
        generatePath(absolutePath,curPath->rootPath,curPath->userPath);
        printf("absolutePath=%s\n",absolutePath);
        visitStack(s);
        //    ERROR_CHECK(ret,-1,"cd");
    }
    stackDestory(s);
    return 0;
}
