#include<func.h>
//typedef struct stack_s{
//    int top;//控制栈顶
//    char* elements[500];//存放入栈的字符串
//}Stack_t;

typedef  struct path_s{
    char* root;//定死的用户根目录
    char* path;//客户端的目录
}path_t;
//简化路径 如/w/../a/./../b/c化简为/b/c,并重新赋值给path
//int simplifyPath(Stack_t* s,char * path){
//    char* str=strtok(path,"/");
//    while(str){
//        if(strcmp(str,"..")==0)
//        {
//            if(s->top!=0)
//            {
//                bzero(s->elements[s->top-1],strlen(s->elements[s->top-1]));
//                s->top--;   //出栈
//            }
//        }
//        else if(strcmp(str,".") == 0)
//        {
//        }
//        else
//        {
//            s->elements[s->top]=str;
//            s->top++;       //入栈
//        }
//        str=strtok(NULL,"/");
//    }
//    char* res=(char*)calloc(512,1);
//
//    for(int i=0;i<s->top;i++){
//        strcat(res,"/");
//        strcat(res,s->elements[i]);
//    }
//    memcpy(path,res,strlen(res));
//    path[strlen(res)]='\0';
//    free(res);
//    return 0;
//}
//
////生成绝对路径
//int generatePath(char* absolutePath,const char* root,const char* path){
//    strcpy(absolutePath,root);
//    strcat(absolutePath,path);
//    return 0;
//}

int list(path_t rootuser ,const char* pathpara, int netfd)
{
    char abspath[4096] = {0};
    sprintf(abspath,"%s%s",rootuser.root,rootuser.path);
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
    sprintf(abspath,"%s%s",rootuser.root,rootuser.path);
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
            return -1;
        }
    }
    send(netfd,"Remove Done!",5,0);

    return 0;
}
int main(void)
{
    path_t rootuser;
    char buf[128] = {"/home/chenchao/wangdao/linuxday_25"};
    char buf2[128] = {"/project"};
    rootuser.root = buf;
    rootuser.path = buf2;

    //list(rootuser,"newfile",1);
    removeThing(rootuser,"newfile",2);
    return 0;
}
