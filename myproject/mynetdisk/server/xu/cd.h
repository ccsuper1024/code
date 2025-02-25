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
#define STACKLENTH 64
typedef struct stack_s{
    int top;//控制栈顶
    char* elements[STACKLENTH];//存放入栈的字符串
}Stack_t;

typedef  struct path_s{
    char* rootPath;//定死的用户根目录
    char* userPath;//客户端的目录
}path_t;
//栈初始化和销毁
Stack_t* stackInit();
int stackDestory(Stack_t* stack);

//cd命令传入栈，路径结构体和用户发来的cd 参数
int cd(Stack_t* stack,path_t* curPath,char* str);
//生成绝对路径
int generatePath(char* absolutePath, char* rootPath,char* path);

//pwd命令处理了发送行为
int pwd(int netfd,const path_t* curPath);
