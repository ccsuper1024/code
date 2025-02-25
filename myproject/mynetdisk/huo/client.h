#pragma once

#define LOG_MESSAGE(priority, message) \
    openlog("my_program", LOG_PID | LOG_CONS, LOG_USER); \
    syslog(priority, "file:%s,func:%s,line:%d,操作:%s",__FILE__,__FUNCTION__,__LINE__, message); \
    closelog();

/*
LOG_EMERG:紧稳情况，系统无法使用。
LOG_ALERT: 必须立即采取行动。
LOG_CRIT: 关键情况，例如硬件故障。
LOG_ERR:错误情况。
LOG_WARNING:警告情况。
LOG_NOTICE: 正常但重要的情况。
LOG_INFO]:信息性消息。
LOG_DEBUG:调试消息。
*/



#include"func.h"
#include "head.h"
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

typedef struct train_s{
    int length;
    char data[1000];
} train_t;

int analysis(char* buf,int *cmd,char *path);
int clientRecvFile(int sockfd,const char* filename);
int upLoad(int sockfd, char *filename);
