#pragma once
#include<func.h>

typedef struct tidArr_s{
    pthread_t *arr;/*线程id数组*/
    int workerNum;
}tidArr_t;

int tidArrInit(tidArr_t*ptidArr,int workerNum);
void *threadFun(void *arg);
void unlock(void *arg);
