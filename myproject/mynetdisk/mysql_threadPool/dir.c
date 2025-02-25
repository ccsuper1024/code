#include"threadPool.h"
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

int mkDir(char* filepath){
    int ret=mkdir(filepath,0777);
    return ret;
}

//============================================================chenchao===============================//
int list(path_t rootuser ,const char* pathpara, int netfd)
{
    char abspath[4096] = {0};
    sprintf(abspath,"%s%s",rootuser.rootPath,rootuser.userPath);
    strcat(abspath,"/");
    strcat(abspath,pathpara);
    puts(abspath);

    //定义目录指针，和目录项指针
    DIR         *dp;
    struct dirent      *dirp;
    char buf[4096];
    bzero(buf,sizeof(buf));
    //打开目录文件
    if((dp = opendir(abspath)) == NULL)
    {
        send(netfd,"ls wrong",8,0);
        return -1;
    }
    //遍历目录项
    while((dirp = readdir(dp)) != NULL)
    {
        //向客户端发送目录项中的名字
        strcat(buf,dirp->d_name);
        strcat(buf,"\n");
    }
    puts(buf);
    send(netfd,buf,sizeof(buf),0);
    
    closedir(dp);
    return 0;
}
//int pwd(int netfd)
//{
//    //获取当前目录
//    char *buf = getcwd(NULL,0);
//    //裁剪出客户端的目录
//    char filepath[128]; 
//    strcpy(filepath,buf+strlen(root));  //
//    puts(filepath);
//
//    //项客户端发送当前目录
//    ssize_t sret = send(netfd,buf,strlen(buf),0);
//    ERROR_CHECK(sret,-1,"write");
//
//    free(buf);
//    return 0;
//}
static int recurdelete(const char* path)
{
    //定义目录指针和目录项指针
    DIR           *dp;
    struct dirent *dirp;
    int ret = 0;
    if(( dp = opendir(path)) == NULL)
        return -1;

    while((dirp = readdir(dp)) != NULL)
    {
        //删除目录时不应该删除.和..
        char*  name = dirp->d_name;
        if(strcmp(name,".") == 0|| strcmp(name,"..") == 0)
            continue;
        //拼接子路径
        char subpath[1024] = {0};
        sprintf(subpath,"%s/%s",path,dirp->d_name);

        if(dirp->d_type == DT_DIR)
        {
            if((ret = recurdelete(subpath)) == -1)
                return -1;
        }
        else
        {
            if((ret = unlink(subpath)) == -1)
                return -1;
        }
    }
    if((ret = rmdir(path)) == -1)
        return -1;

    closedir(dp);
    return 0;
}
int removeThing(path_t rootuser ,const char* pathpara,int netfd)
{
    char abspath[4096] = {0}; //= {"/home/chenchao/wangdao/linuxday_25/project/newfile"};
    //生成绝对路径
    sprintf(abspath,"%s%s",rootuser.rootPath,rootuser.userPath);
    strcat(abspath,"/");
    strcat(abspath,pathpara);
    
    struct stat statbuf;//线程安全
    int ret = stat(abspath,&statbuf);
    if(ret == -1)
        send(netfd,"wrong path",10,0);
    if(S_ISDIR(statbuf.st_mode))
    {
        if((ret = recurdelete(abspath)) == -1)    
        {    
            send(netfd,"remove fail",11,0);
            return -1;
        }
    }
    else if(S_ISREG(statbuf.st_mode))
    {
        if((ret = unlink(abspath)) == -1 )   
        {   
            send(netfd,"remove fail",11,0);
        }
    }
    send(netfd,"Remove Done!",10,0);

    return 0;
}
