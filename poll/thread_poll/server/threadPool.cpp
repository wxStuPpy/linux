#include "threadPool.h"
#include "worker.h"

int threadPoolInit(threadPool_t *pthreadPool, int workerNum)
{
    tidArrInit(&pthreadPool->tidArr, workerNum);
    taskQueueInit(&pthreadPool->taskQueue);
    pthread_mutex_init(&pthreadPool->mutex,nullptr);
    pthread_cond_init(&pthreadPool->cond,nullptr);
    return 0;
}
int makeWorker(threadPool_t*pthreadPool){
    for(int i=0;i<pthreadPool->tidArr.workerNum;++i){
        pthread_create(&pthreadPool->tidArr.arr[i],nullptr,threadFun,pthreadPool);
    }
    return 0;
}