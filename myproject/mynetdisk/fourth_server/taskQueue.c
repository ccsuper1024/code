#include "taskQueue.h"
int Enqueue(queue_t *pqueue,int fd,int flag,char* cmdStr,int dirId){
    node_t *newNode = (node_t *)calloc(1,sizeof(node_t));
    newNode->fd = fd;
    newNode->flag=flag;
    newNode->dirId=dirId;
    memcpy(newNode->cmdStr,cmdStr,strlen(cmdStr));
    if(pqueue->queueSize == 0){
        pqueue->pfront = pqueue->prear = newNode;
    }
    else{
        pqueue->prear->next = newNode;
        pqueue->prear = newNode;
    }
    pqueue->queueSize++;
    return 0;
}
int Dequeue(queue_t *pqueue){
    node_t *pcur = pqueue->pfront;
    pqueue->pfront = pcur->next;
    free(pcur);
    pqueue->queueSize--;
    if(pqueue->queueSize == 0){
        pqueue->prear = NULL;
    }
    return 0;
}

