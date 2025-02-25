#include <func.h>
#include <mychat.h>

typedef struct shaRes_s{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shaRes_t;

int* pid;

void handler(int signum){
    if(signum == SIGINT){
        for(int i = A;i <= B1;i++){
            kill(pid[i],SIGUSR1);
        }
    }
    else if(signum == SIGUSR1){
        printf("end!\n");
        shm_unlink("/myshmm");
        shm_unlink("/pid");
        pid[A1] = 0;
        exit(0);
    }
}

//A1进程负责显示B发送给A的消息
//A和B通过有名管道通信，A1和A用共享内存通信
int main(void)
{
    //将信号注册
    signal(SIGINT,handler);
    signal(SIGUSR1,handler);

    int fd_of_pid = shm_open("/pid",O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd_of_pid,-1,"shm_open pid");
    int err = ftruncate(fd_of_pid,sizeof(int) * MAX_OF_PID);
    ERROR_CHECK(err,-1,"ftruncate");
    pid = (int*)mmap(NULL,sizeof(int) * MAX_OF_PID,PROT_READ|PROT_WRITE,MAP_SHARED,fd_of_pid,0);
    ERROR_CHECK(pid,MAP_FAILED,"mmap");
    pid[A1] = getpid();
    //等待4个进程都开启
    while(1){
        if(pid[A] && pid[A1] && pid[B] && pid[B1])
            break;
    }
    printf("test!\n");

    int shmid = shmget(0x0077,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    char* p = (char*)shmat(shmid,NULL,0);

    //使用共享内存获取和A相同的一把锁
    int fd = shm_open("/myshmm",O_RDWR,0666);
    ERROR_CHECK(fd,-1,"shm_open");

    err = ftruncate(fd,sizeof(shaRes_t));
    ERROR_CHECK(err,-1,"ftruncate")

    shaRes_t* shaRes = (shaRes_t*)mmap(NULL,sizeof(shaRes_t),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(shaRes,MAP_FAILED,"mmap");

    //直接打印A写进共享内存中的信息
    while(1){
        pthread_mutex_lock(&shaRes->mutex);
        if(shaRes->flag != 1){
            pthread_cond_wait(&shaRes->cond,&shaRes->mutex);
        }
        puts(p);
        shaRes->flag = 0;
        memset(p,0,4096);
        pthread_mutex_unlock(&shaRes->mutex);
    }

    shmdt(p);
    shm_unlink("/myshmm");
    return 0;
}

