#include <func.h>
#include <mychat.h>

char* p;
int* pid;

void handler(int signum){
    if(signum == SIGINT){
        for(int i = A;i <= B1;i++){
           kill(pid[i],SIGUSR1);
        }
    }
    else if(signum == SIGUSR1){
        for(int i = A;i <= B1;i++){
            if(i == A)
                continue;
            else if(pid[i]){
                kill(pid[i],SIGUSR1);
            }
        }
        pid[A] = 0;
        printf("end!\n");
        shm_unlink("/myshmm");
        int shmid = shmget(0x0077,4096,IPC_CREAT|0600);
        ERROR_CHECK(shmid,-1,"shmget");
        shmctl(shmid,IPC_RMID,NULL);
        exit(0);
    }
}

typedef struct shaRes_s{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shaRes_t;

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);

    int fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open fdr");
    int fdw = open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open fdw");

    //将信号注册
    signal(SIGINT,handler);
    signal(SIGUSR1,handler);
    
    //用共享内存让A1和A都拿到同一把锁
    int fd = shm_open("/myshmm",O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd,-1,"shm_open");

    int err = ftruncate(fd,sizeof(shaRes_t));
    ERROR_CHECK(err,-1,"mmap");

    shaRes_t* shaRes = (shaRes_t*)mmap(NULL,sizeof(shaRes_t),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(shaRes,MAP_FAILED,"mmap");

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shaRes->mutex,&attr);
    pthread_condattr_t condattr;
    pthread_condattr_init(&condattr);
    pthread_condattr_setpshared(&condattr,PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&shaRes->cond,&condattr);
    shaRes->flag = 0;

    //将4个进程的pid都放到同一个共享内存
    int fd_of_pid = shm_open("/pid",O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd_of_pid,-1,"shm_open pid");
    err = ftruncate(fd_of_pid,sizeof(int) * MAX_OF_PID);
    ERROR_CHECK(err,-1,"ftruncate");
    pid = (int*)mmap(NULL,sizeof(int) * MAX_OF_PID,PROT_READ|PROT_WRITE,MAP_SHARED,fd_of_pid,0);
    ERROR_CHECK(pid,MAP_FAILED,"mmap");
    pid[A] = getpid();
    //等待4个进程都开启
    while(1){
        if(pid[A] && pid[A1] && pid[B] && pid[B1])
            break;
    }
    printf("All begin!\n");

    //链接与A1进行通信的共享
    int shmid = shmget(0x0077,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
    p = (char*)shmat(shmid,NULL,0);


    pthread_mutex_lock(&shaRes->mutex);
    memset(p,0,4096);
    pthread_mutex_unlock(&shaRes->mutex);

    fd_set rdset;
    while(1){
        FD_ZERO(&rdset);

        FD_SET(fdr,&rdset);
        FD_SET(STDIN_FILENO,&rdset);

        select(fdw+1,&rdset,NULL,NULL,NULL);

        char buf[256];
        if(FD_ISSET(fdr,&rdset)){
            memset(buf,0,sizeof(buf));
            ssize_t sret = read(fdr,buf,sizeof(buf) - 1);
            if(sret == 0){
                printf("被优雅地结束\n");
                break;
            }
            ERROR_CHECK(sret,-1,"read fdr");
            buf[sret] = '\0';

            //把读到的信息写入共享内存中
            pthread_mutex_lock(&shaRes->mutex);
            memset(p,0,4096);
            strcpy(p,buf);
            shaRes->flag = 1;
            pthread_cond_signal(&shaRes->cond);
            pthread_mutex_unlock(&shaRes->mutex);
            sleep(1);
        }
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            memset(buf,0,sizeof(buf));
            ssize_t sret = read(STDIN_FILENO,buf,sizeof(buf));
            ERROR_CHECK(sret,-1,"read STDIN");
            ssize_t sret2 = write(fdw,buf,strlen(buf));
            ERROR_CHECK(sret2,-1,"write fdw");
            if(sret == 0){
                printf("优雅地结束\n");
                break;
            }
        }
    }


    shmdt(p);
    shm_unlink("/myshmm");
    close(fdw);
    close(fdr);
    return 0;
}

