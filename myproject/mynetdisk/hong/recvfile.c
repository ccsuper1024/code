#include "head.h"

//确保该用户ID对应的表中数据一定要存在,userName为空数组
int getUserName(MYSQL* mysql,user_t* usInfo,char* userName){
    char sql[100]={0};
    sprintf(sql,"%s%d%s","select userName from usertable where id=",usInfo->userId,";");
    int qret=mysql_query(mysql,sql);

    if(qret != 0){
        fprintf(stderr,"error:%s\n", mysql_error(mysql));
        return -1;
    }

    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    memcpy(userName,row[0],strlen(row[0]));
    return 0;
}

//获得当前文件所在虚拟路径(未包含此文件名)
int getCurPath(MYSQL *mysql, int curDirId, char *curPath)
{
    char sql[100];
    sprintf(sql,"select path from filetable where id = '%d';", curDirId);
    printf("sql = %s\n",sql);
    int qret = mysql_query(mysql, sql);
    if(qret != 0)
    {
        fprintf(stderr, "error:%s", mysql_error(mysql));

        return -1;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    printf("row = %s,size = %d,len = %d\n",row[0], sizeof(row[0]), strlen(row[0]));
    memcpy(curPath,row[0], strlen(row[0]));
    return 0;
}

//插入文件表
int insertFileTable(MYSQL *mysql, char *name, char *type, char *pre_id, char *path, char *user, char *tomb, char *md5str)
{
    char sql[300];
    memset(sql, 0, sizeof(sql));
    sprintf(sql,
            "insert into filetable (name, type, pre_id, path, user, tomb, md5str) values('%s','%s','%s','%s','%s','%s','%s');"
            ,name,type,pre_id,path,user,tomb,md5str);
    printf("sql = %s\n", sql);
    int qret = mysql_query(mysql, sql);
    if(qret != 0)
    {
        fprintf(stderr, "error:%s", mysql_error(mysql));
        return -1;
    }
    return 0;
}

int dupName(char *filename,int curPathId, MYSQL *mysql)
{
    char sql[200];
    sprintf(sql,"select * from filetable where pre_id = '%d' and name = '%s' and tomb = '0';",curPathId,filename);
    int qret = mysql_query(mysql,sql);
    if(qret != 0)
    {
        fprintf(stderr, "error:%s", mysql_error(mysql));
        return -1;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    int row = mysql_num_rows(res);
    return row;
}

int recvn(int sockfd, void *buf, int length)
{
    char *pbuf = (char *)buf;
    int total = 0;
    while(total < length)
    {
        int ret = recv(sockfd, pbuf + total, length - total, 0);
        if(ret == 0)//收到0则退出
        {
            printf("recv 0 bytes\n");
            break;
        }
        total += ret;
        printf("total = %d\n", total);
        sleep(1);
    }
    if(total == length)
        return 0;
    else
        return total;
}

int recvFile(int sockfd, char *filename, user_t *user, MYSQL *mysql)
{
    int ret;

    //查询是否有同名文件
    int num  = dupName(filename,user->curDirId,mysql);
    send(sockfd, &num, sizeof(num), 0);
    if(num != 1)
    {
        int isopen;
        recvn(sockfd, &isopen, sizeof(isopen));
        if(isopen != -1)
        {


            //接收上传文件的MD5
            char decrypt32[33]; 
            recvn(sockfd, decrypt32, sizeof(decrypt32));
            printf("dec = %s\n",decrypt32);

            //数据库文件表查询MD5
            char sql[200];
            sprintf(sql,"select * from filetable where md5str = '%s';", decrypt32);
            printf("sql = %s\n",sql);
            int qret = mysql_query(mysql, sql);
            if(qret != 0)
            {
                fprintf(stderr, "error:%s\n", mysql_error(mysql));
                return -1;
            }
            MYSQL_RES *res = mysql_store_result(mysql);
            int row = mysql_num_rows(res);

            char curPath[50];
            char openPath[50];//创建文件使用
            memset(curPath,0,sizeof(curPath));
            memset(openPath,0,sizeof(curPath));
            ret = getCurPath(mysql, user->curDirId, curPath);//查询当前path
            ERROR_CHECK(ret, -1, "getCurPath");
            memcmp(openPath,curPath,sizeof(openPath));
            if(strcmp(curPath,"/"))
            {
                strcat(curPath,"/");
            }
            strcat(curPath, filename);//拼接为文件path
            printf("curPath = %s\n", curPath);

            char userName[20];
            ret = getUserName(mysql, user ,userName);
            ERROR_CHECK(ret, -1, "getUserName");

            char curPathId[10];
            sprintf(curPathId, "%d", user->curDirId);

            //插入表项
            printf("decrypt = %s\n",decrypt32);
            ret = insertFileTable(mysql, filename, "f", curPathId, curPath, userName, "0", decrypt32);
            ERROR_CHECK(ret, -1, "insertFileTable");

            //创建或打开文件
            if(strcmp(openPath,"/"))
            {
                strcat(openPath,"/");
            }
            char realFileName[100];
            memset(realFileName, 0, sizeof(realFileName));
            sprintf(realFileName,"%s%s%s",user->rootPath,openPath,decrypt32);
            printf("realfilename = %s\n",realFileName);
            int fd = open(realFileName, O_RDWR | O_CREAT, 0666);
            ERROR_CHECK(fd, -1, "open");

            //服务端发送当前文件长度(没有则为0，已有则为文件长度)
            int curSize;
            struct stat statbuf;
            ret = fstat(fd, &statbuf);
            ERROR_CHECK(ret, -1, "fstat");
            curSize = statbuf.st_size;
            ret = send(sockfd, &curSize, sizeof(curSize), MSG_NOSIGNAL);
            ERROR_CHECK(ret, -1, "send");
            printf("cursize = %d\n", curSize);

            //接收用户上传文件总长度
            int fileLength; 
            recvn(sockfd, &fileLength, sizeof(fileLength));
            printf("filelength = %d\n", fileLength);

            //truncate 文件
            ret = ftruncate(fd, fileLength);
            printf("filelength = %d\n", fileLength);
            char *pFile = (char *)mmap(NULL, fileLength, PROT_WRITE | PROT_READ,MAP_SHARED, fd, 0); 
            ERROR_CHECK(pFile, MAP_FAILED, "mmap");

            //接收文件内容
            ret = recvn(sockfd, pFile + curSize, fileLength - curSize);
            if(ret != 0)
                ftruncate(fd, curSize + ret);
            printf("recvFile success\n");
        }
    }
    return 0;
}
