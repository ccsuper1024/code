#include <func.h>
#include <mysql/mysql.h>

typedef struct user_s{
    int userId;
    int curDirId;
    int rootDirId;
    char rootPath[24];
}user_t;

typedef struct stack_s{
    int top;//控制栈顶
    char* elements[64];//存放入栈的字符串
}Stack_t;


 int getCurPath(MYSQL *mysql, int curDirId, char *curPath)
 {
     char sql[100];
     sprintf(sql,"select path from filetable where id = %d;", curDirId);
     printf("sql = %s\n",sql);
     int qret = mysql_query(mysql, sql);
     if(qret != 0){
         return -1;
     }
     MYSQL_RES *res = mysql_store_result(mysql);
     MYSQL_ROW row;
     row = mysql_fetch_row(res);
     memcpy(curPath,row[0], strlen(row[0]));
     return 0;
 }
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
    char sql[128]={0};
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
    sprintf(sql,"insert into filetable values (null,'%s','d',%d,'%s/%s','%s',0,null);",
            str,userInfo->curDirId,path,str,userName);
    printf("sql=%s\n",sql);
    int iret= mysql_query(mysql,sql);
    if(iret==-1){
        return -1;
    }
    return 0;
}


int main(){
    user_t userInfo;
    userInfo.userId=1;
    userInfo.rootDirId=0;
    userInfo.curDirId=0;
    memcpy(userInfo.rootPath,"/home/yangjunjie/client",24);
    printf("path=%s\n",userInfo.rootPath);
    MYSQL * mysql = mysql_init(NULL);
    MYSQL * ret = mysql_real_connect(mysql,"localhost","root","xufuzhou123","test",0,NULL,0);
    if(ret == NULL){
        fprintf(stderr,"error:%s\n", mysql_error(mysql));
        return -1;
    }
    char userName[40]={0};
    getUserName(mysql,&userInfo,userName);
    printf("userName=%s\n",userName);
    curDirInit(mysql,&userInfo);
    printf("rootdirid=%d,curdirid=%d\n",userInfo.rootDirId,userInfo.curDirId);
    char testDir[]="/dir1/dir4/";
    int cdret =cdDir(mysql,&userInfo,testDir);
    if(cdret==-1){
        printf("error in cd\n");
    }
    printf("rootdirid=%d,curdirid=%d\n",userInfo.rootDirId,userInfo.curDirId);
    char testDir2[12]="dir5";
    int mkdirret=mkDir(mysql,&userInfo,testDir2);
    printf("mkdirret=%d\n",mkdirret);
    mysql_close(mysql);
    return 0;
}
