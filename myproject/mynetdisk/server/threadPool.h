#ifndef __THREAD_POLL__
#define __THREAD_POLL__
#include <func.h>
#include "taskQueue.h"
#include <mysql/mysql.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include "md5.h"
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
typedef struct threadPoll_s{
    pthread_t *tidArr;
    int workerNum;
    queue_t taskQueue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int exitflag;
}threadPool_t;
typedef struct user_s{
    int userId;
    int rootDirId;
    int curDirId;
    char rootPath[24];
}user_t;

int curidx;//记录连接了多少个客户
void* threadFunc(void* arg);
int tcpInit(const char* ip,const char* port,int *psockfd);
int epollAdd(int epfd,int fd);
int epollDel(int epfd,int fd);
int makeworker(threadPool_t *pthreadPool);
int recvCommand(int netfd,int *command,char *buff,train_t *train);
int passwdCrypted(const char *name,const char *passwd);
//chenjiangling
int serverSendFile(int netfd,const char* filename,user_t* puser,MYSQL* mysql);
//hongyabo
int upLoad(int sockfd, char *filename);                                       
int recvFile(int sockfd, char *filename, user_t *user, MYSQL *mysql);
int getCurPath(MYSQL *mysql, int curDirId, char *curPath);
//xufuzhou

//初始化curDirId和rootDirId，若表中没有则创建并查询获取;
int curDirInit(MYSQL* mysql,user_t* usInfo);

//确保该用户ID对应的表中数据一定要存在,userName为空数组
int getUserName(MYSQL* mysql,user_t* usInfo,char* userName);

//cd功能
int cdDir(MYSQL* mysql,user_t* userInfo,char* str);
//PWD处理了发送行为
int pwd(MYSQL*mysql,user_t* userInfo,int netfd);
int mkDir(MYSQL*mysql,user_t* userInfo,char* str);

//chenchao
int list(user_t userinfo,MYSQL* sqlhandler ,const char* userbuff, int netfd);
int removeThing(user_t userinfo ,MYSQL* sqlhandler,const char* userbuff,int netfd);
int GenerateStr(char* salt);
#endif
