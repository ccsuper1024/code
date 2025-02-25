#ifndef _HEAD
#define _HEAD
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <func.h>
#include <sys/mman.h>
#include <mysql/mysql.h>
typedef struct user_s{
    int userId;
    int curDirId;
    const char rootPath[90];
}user_t;
int upLoad(int sockfd, char *filename);
int recvFile(int sockfd, char *filename, user_t *user, MYSQL *mysql);
int recvn(int sockfd, void* buf,int length);
#endif
