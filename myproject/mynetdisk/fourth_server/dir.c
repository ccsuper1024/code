#include "threadPool.h"

typedef struct stack_s{
    int top;//控制栈顶
    char* elements[64];//存放入栈的字符串
}Stack_t;


//根据查询结果将第一列的值赋值改变curDirId
int changeCurID(MYSQL* mysql,char* sql,user_t* userInfo){
    mysql_query(mysql,sql);
    MYSQL_RES *res = mysql_store_result(mysql);
    int rowNum=mysql_num_rows(res);
    if(rowNum==0){
        return -1;
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    userInfo->curDirId=atoi(row[0]);
    return 0;
}
//简化绝对路径
int simplifyPath(Stack_t * s,char * path){
    char* str=strtok(path,"/");
    while(str){
        if(strcmp(str,"..")==0){
            if(s->top!=0){
                s->elements[s->top-1]=NULL;
                s->top--;
            }
        }else if(strcmp(str,".")==0){
        }else{
            s->elements[s->top]=str;
            s->top++;
        }
        str=strtok(NULL,"/");
    }
    return 0;
}

//简化普通路径并按顺序入栈
int simplify(Stack_t* s,char* path){
    char* str=strtok(path,"/");
    while(str){
        s->elements[s->top]=str;
        s->top++;
        str=strtok(NULL,"/");
    }
    return 0;

}

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
//初始化curDirId和rootDirId，若表中没有则创建并查询获取;
int curDirInit(MYSQL* mysql,user_t* usInfo){
    char userName[40]={0};
    getUserName(mysql,usInfo,userName);
    char sql[128]={0};
    sprintf(sql,"%s%s%s","select * from filetable where pre_id=-1 and user='",userName,"';");
    int qret=mysql_query(mysql,sql);

    if(qret != 0){
        fprintf(stderr,"error:%s\n", mysql_error(mysql));
        return -1;
    }

    MYSQL_RES *res = mysql_store_result(mysql);
    if(mysql_num_rows(res)==0){
        //不存在则插入
        bzero(sql,sizeof(sql));
        sprintf(sql,"%s%s%s","insert into filetable values (null,'/','d',-1,'/','",userName,"',0,null);");
        mysql_query(mysql,sql);
        //res = mysql_store_result(mysql);
        //根据插入结果获取数据
        bzero(sql,sizeof(sql));
        sprintf(sql,"%s%s%s","select * from filetable where user='",userName,"';");
        mysql_query(mysql,sql);
        printf("sql=%s\n",sql);
        res = mysql_store_result(mysql);
    }
    //根据结果来赋值
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    usInfo->curDirId=atoi(row[0]);
    usInfo->rootDirId=atoi(row[0]);
    return 0;
}


int cdDir(MYSQL* mysql,user_t* userInfo,char* str){
    Stack_t* stack=(Stack_t*)calloc(1,sizeof(Stack_t));
    char sql[128]={0};
    
    int tmp=userInfo->curDirId;//备份一下
    if (str[0]=='/'){//绝对路径的话
        simplifyPath(stack,str);//简化
        /* visitStack(stack);//测试用句 */
        if(stack->top==0){
            userInfo->curDirId=userInfo->rootDirId;
            return 0;
        }
        //先根据根目录找到第一个
        bzero(sql,sizeof(sql));
        sprintf(sql,"%s%d%s%s%s","select * from filetable where pre_id=",userInfo->rootDirId," and name='",stack->elements[0],"';");
        int ret=changeCurID(mysql,sql,userInfo);
        if(ret==-1){
            userInfo->curDirId=tmp;
            free(stack);
            return -1;
        } 
        //根据栈内元素寻找下一个目录
        for(int i=1;i<stack->top;i++){
            bzero(sql,sizeof(sql));
            sprintf(sql,"%s%d%s%s%s","select * from filetable where pre_id=",userInfo->curDirId," and name='",stack->elements[i],"';");
            int ret=changeCurID(mysql,sql,userInfo);
            if(ret==-1){
            userInfo->curDirId=tmp;
                free(stack);
                return -1;
            } 
        }
    }else{//输入的是普通路径
        simplify(stack,str);//只做简单分解
        for(int i=0;i<stack->top;i++){
            bzero(sql,sizeof(sql));
            if(strcmp(stack->elements[i],"..")==0){//退回一级目录,根据当前id找pre_id
                sprintf(sql,"%s%d%s","select pre_id from filetable where id=",userInfo->curDirId,";");
                int ret=changeCurID(mysql,sql,userInfo);
                if(ret==-1){
                    userInfo->curDirId=tmp;
                    free(stack);
                    return -1;
                } 
            }else if(strcmp(stack->elements[i],".")==0){
                continue;
            }else{
                sprintf(sql,"%s%d%s%s%s","select * from filetable where pre_id=",userInfo->curDirId," and name='",stack->elements[i],"';");
                int ret=changeCurID(mysql,sql,userInfo);
                if(ret==-1){
                    userInfo->curDirId=tmp;
                    free(stack);
                    return -1;
                } 
            }

        }
    }
    free(stack);
    return 0;
}

int pwd(MYSQL*mysql,user_t* userInfo,int netfd){
    char sql[128]={0};
    char path[128]={0};
    sprintf(sql,"%s%d%s","select path from filetable where id=",userInfo->curDirId,";");
    mysql_query(mysql,sql);
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    memcpy(path,row[0],strlen(row[0]));
    send(netfd,path,strlen(path),0);
    return 0;
}

int mkDir(MYSQL*mysql,user_t* userInfo,char* str){
    char sql[256]={0};
    sprintf(sql,"%s%d%s","select name from filetable where pre_id=",userInfo->curDirId,";");
    printf("sql=%s\n",sql);
    mysql_query(mysql,sql);
    MYSQL_RES* res=mysql_store_result(mysql);
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res)) != NULL){
        for(unsigned int i=0;i<mysql_num_fields(res);i++){//无符号整数
            if(strcmp(str,row[i])==0){
                return -1;
            }
        }
    }

    //插入
    bzero(sql,sizeof(sql));
    char path[128]={0};
    getCurPath(mysql,userInfo->curDirId,path);
    char userName[40]={0};
    getUserName(mysql,userInfo,userName);
    if(strcmp(path,"/")==0){
        bzero(path,sizeof(path));
    }
    sprintf(sql,"insert into filetable values (null,'%s','d',%d,'%s/%s','%s',0,null);",
            str,userInfo->curDirId,path,str,userName);
    printf("sql=%s\n",sql);
    int iret= mysql_query(mysql,sql);
    if(iret==-1){
        return -1;
    }
    return 0;
}


int getCurPath(MYSQL *mysql, int curDirId, char *curPath)
{
    char sql[100];
    sprintf(sql,"select path from filetable where id = %d;", curDirId);
    printf("sql = %s\n",sql);
    int qret = mysql_query(mysql, sql);
    if(qret != 0)
    {
        fprintf(stderr, "error:%s\n", mysql_error(mysql));

        return -1;
    }
    MYSQL_RES *res = mysql_store_result(mysql);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    memcpy(curPath,row[0], strlen(row[0]));
    return 0;
}
int list(user_t userinfo,MYSQL* sqlhandler ,const char* userbuff, int netfd)
{
    //获取username
    char username[100];
    //从用户id获取对应的用户名
    int ret = getUserName(sqlhandler,&userinfo,username);
    if(ret == -1)
        return -1;
    //定义sql语句
    char sql[1024];
    memset(sql,0,sizeof(sql));
    //get sql
    if(userbuff == 0)
    {
        sprintf(sql,"%s%s%s%d;","select name from filetalbe where user='"
                ,username,"' AND pre_id = ",userinfo.curDirId);
    }
    else
    {
        char path[100];
        getCurPath(sqlhandler,userinfo.curDirId,path);
        strcat(path,"/");
        strcat(path,userbuff);//获取新的路径
        sprintf(sql,"%s%s%s%s;","select name from filetalbe where user='",
                username,"' AND path = ",path);
    }

    //调用mysql_query
    ret = mysql_query(sqlhandler,sql);
    if(ret != 0){
        fprintf(stderr,"error:%s\n", mysql_error(sqlhandler));
        send(netfd,"ls wrong",8,0);
        return -1;
    }

    //调用mysql_store_result,获取所有相同目录下所有文件
    MYSQL_RES* res = mysql_store_result(sqlhandler);
    if(res == NULL)
    {
        fprintf(stderr,"error: %s\n",mysql_error(sqlhandler));
        send(netfd,"ls wrong",8,0);
        return -1;
    }

    //把文件名发送给客户端
    int rowNum=mysql_num_rows(res);
    if(rowNum==0){
        send(netfd,"empty dir!",10,0);
        return 0;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    
    while((row=mysql_fetch_row(res))!=NULL){
        send(netfd,row[0],sizeof(row[0]),0);
        send(netfd,"\n",2,0);
    }

    return 0;
}
int removeThing(user_t userinfo ,MYSQL* sqlhandler,const char* userbuff,int netfd)
{
    char abspath[512] = {0}; //= {"/home/chenchao/wangdao/linuxday_25/project/newfile"};
    //生成绝对路径
    char username[100];
    int ret = getUserName(sqlhandler,&userinfo,username);//获取用户名
    if(ret == -1)
    {
        send(netfd,"remove wrong",12,0);
        return -1;
    }
    ret = getCurPath(sqlhandler,userinfo.curDirId,abspath);   //获取当前的路径名
    if(ret == -1)
    {
        send(netfd,"remove wrong",12,0);
        return -1;
    }
    strcat(abspath,"/");
    strcat(abspath,userbuff);

    //从数据库中更新对应用户的对应路径名的tomb值
    char sql[1024]={0};
    memset(sql,0,sizeof(sql));
    sprintf(sql,"select * from filetable where tomb=0 and name='%s' and path='%s';",
            username,abspath);
    mysql_query(sqlhandler,sql);
    MYSQL_RES* res=mysql_store_result(sqlhandler);
    int rowNum=mysql_num_rows(res);
    if(rowNum==0){
        send(netfd,"no such file or dir!",20,0);
        return -1;
    }
    bzero(sql,sizeof(sql));
    sprintf(sql,"UPDATE filetable SET tomb = -1  where name ='%s' and path='%s';"
            ,username,abspath);

    //调用mysql_query                                                                                      
    ret = mysql_query(sqlhandler,sql);
    send(netfd,"Remove Done!",11,0);

    return 0;
}

