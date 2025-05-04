#include"taskQueue.h"

int taskQueueInit(taskQueue_t*ptaskQueue){
    ptaskQueue->pFront=nullptr;
    ptaskQueue->pRear=nullptr;
    ptaskQueue->queueSize=0;
    return 0;
}