#ifndef __THREAD_POLL__
#define __THREAD_POLL__
#include <func.h>
#include "taskQueue.h"
enum command{
    CD,
    LS,
    PUTS,
    GETS,
    REMOVE,
    PWD,
    RMDIR,
    MKDIR
};
#define STACKLENGTH 64
#define STACKLENTH 64
//message为字符串,使用itoa转换
#define LOG_MESSAGE(priority, message) \
    openlog("my_program", LOG_PID | LOG_CONS, LOG_USER); \
    syslog(priority, "file:%s,func:%s,line:%d,操作:%s",__FILE__,__FUNCTION__,__LINE__, message); \
    closelog();
typedef struct train_s{
    int length;
    char data[1000];
}train_t;
typedef struct path_s{
    char *userPath;//客户端的目录
    char *rootPath;//定死的用户根目录
}path_t;
typedef struct Stack_s{
    char *elements[STACKLENGTH];//存放入栈的字符串
    int top;//控制栈顶
}Stack_t;
typedef struct threadPoll_s{
    pthread_t *tidArr;
    int workerNum;
    queue_t taskQueue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int exitflag;
}threadPool_t;
int curidx;//记录连接了多少个客户
void* threadFunc(void* arg);
int tcpInit(const char* ip,const char* port,int *psockfd);
int epollAdd(int epfd,int fd);
int epollDel(int epfd,int fd);
int makeworker(threadPool_t *pthreadPool);
int recvCommand(int netfd,int *command,char *buff,train_t *train);
int passwdCrypted(const char *name,const char *passwd);
//chenjiangling
int serverSendFile(int netfd,const char* filename);
//hongyabo
int recvFile(int netfd, char *filename);
//xufuzhou
//栈初始化和销毁
Stack_t* stackInit();
int stackDestory(Stack_t* stack);
//cd命令传入栈，路径结构体和用户发来的cd 参数
int cd(Stack_t* stack,path_t* curPath,char* str);
//生成绝对路径
int generatePath(char* absolutePath, char* rootPath,char* path);
//pwd命令处理了发送行为
int pwd(int netfd,const path_t* curPath);
int mkDir(char* filename);
int simplifyPath(Stack_t * s,char * path);
void visitStack(Stack_t* s);

//chenchao
int list(path_t rootuser ,const char* pathpara, int netfd);
int removeThing(path_t rootuser ,const char* pathpara,int netfd);
#endif
