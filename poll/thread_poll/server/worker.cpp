#include "worker.h"
#include "threadPool.h"
#include "taskQueue.h"

int tidArrInit(tidArr_t*ptidArr,int workerNum){
    ptidArr->arr=new pthread_t[workerNum];
    ptidArr->workerNum=workerNum;
    return 0; 
}

void unlock(void *arg){
    threadPool_t *pthreadPool=(threadPool_t*)arg;
    printf("unlock\n");
    pthread_mutex_unlock(&pthreadPool->mutex);
}

void *threadFun(void *arg){
    threadPool_t *pthreadPool=(threadPool_t*)arg;
    while(1){
        pthread_mutex_lock(&pthreadPool->mutex);
        int netfd;
        //pthread_cleanup_push(unlock,pthreadPool);
        /*队列数量小于0 等待*/
        while(pthreadPool->exitFlag==0&&pthreadPool->taskQueue.queueSize<=0){
            pthread_cond_wait(&pthreadPool->cond,&pthreadPool->mutex);
        }
        if(pthreadPool->exitFlag==1){
            printf("I am child,I am going to exit\n");
            pthread_mutex_unlock(&pthreadPool->mutex);
            pthread_exit(nullptr);
        }
        /*任务队列不会空 出队*/
        netfd=pthreadPool->taskQueue.pFront->netfd;
        printf("I am worker,I got a netfd=%d\n",netfd);
        deQueue(&pthreadPool->taskQueue);
        /*解锁*/
        pthread_mutex_unlock(&pthreadPool->mutex);
        //pthread_cleanup_pop(1);
        /*执行业务*/
        transFile(netfd);
        sleep(10);
        close(netfd);
    }
    pthread_exit(nullptr);
}