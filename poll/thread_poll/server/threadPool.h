#pragma once
#include<func.h>
#include"worker.h"
#include"taskQueue.h"
typedef struct threadPool_s{

    /*所有子线程信息*/
    tidArr_t tidArr;
    /*任务队列*/
    taskQueue_t taskQueue;
    /*锁*/
    pthread_mutex_t mutex;
    /*条件变量*/
    pthread_cond_t cond;
}threadPool_t;

int threadPoolInit(threadPool_t*pthreadPool,int workerNum);
int makeWorker(threadPool_t*pthreadPool);