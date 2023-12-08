#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * 函数执行体。类似java中的Thread#run函数。
 * 当函数体执行完毕，则线程结束。
 * @param arg 创建线程时传入的参数。
 * @return 线程返回的结果，可以被创建该线程的父线程通过pthread_join获取。
 */
void *thread_proc(void *arg)
{
    //申请一个堆上的内存
    //int *result = (int *)malloc(sizeof(int));
    /*for (int i = 0; i < *(int *)arg; ++i)
    {
    }*/
    return arg;
}

int main(void)
{
    setbuf(stdout, NULL);
    pthread_t tid; // 线程id
    int i = 5;
    int errno = pthread_create(&tid, NULL, thread_proc, (void *)i); // 创建线程，并传入一个数字
    if (errno)
    {                                                            // 当线程创建失败，返回的errno非0
        fprintf(stderr, "pthread_create:%s\n", strerror(errno)); // 打印异常信息
        return -1;
    }
    void *val = malloc(sizeof(int));
    errno = pthread_join(tid, &val); // 汇合线程，等待传入tid对应的线程执行结束
    if (errno)
    {                                                          // 当线程汇合失败，返回的errno非0
        fprintf(stderr, "pthread_join:%s\n", strerror(errno)); // 打印异常信息
        return -1;
    }
    printf("线程%lu返回值:%d\n", (unsigned long)tid, (int*)val);
    return 0;
}
