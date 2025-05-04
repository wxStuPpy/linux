#include "worker.h"
#include "threadPool.h"
#include "taskQueue.h"

int tidArrInit(tidArr_t*ptidArr,int workerNum){
    ptidArr->arr=new pthread_t[workerNum];
    ptidArr->workerNum=workerNum;
    return 0; 
}
void *threadFun(void *arg){
    threadPool_t *pthreadPool=(threadPool_t*)arg;
    while(1){
        pthread_mutex_lock(&pthreadPool->mutex);
        /*队列数量小于0 等待*/
        while(pthreadPool->taskQueue.queueSize<=0){
            pthread_cond_wait(&pthreadPool->cond,&pthreadPool->mutex);
        }
        /*任务队列不会空 出队*/
        int netfd=pthreadPool->taskQueue.pFront->netfd;
        printf("I am worker,I got a netfd=%d\n",netfd);
        deQueue(&pthreadPool->taskQueue);
        /*解锁*/
        pthread_mutex_unlock(&pthreadPool->mutex);
        /*执行业务*/
        transFile(netfd);
        close(netfd);
    }
    pthread_exit(nullptr);
}