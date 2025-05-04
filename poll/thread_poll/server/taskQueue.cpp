#include "taskQueue.h"

int taskQueueInit(taskQueue_t *ptaskQueue)
{
    ptaskQueue->pFront = nullptr;
    ptaskQueue->pRear = nullptr;
    ptaskQueue->queueSize = 0;
    return 0;
}

int enQueue(taskQueue_t *ptaskQueue, int netfd)
{
    node_t *pNew = new node_t;
    pNew->netfd = netfd;
    pNew->pnext = nullptr;
    if (ptaskQueue->queueSize == 0)
    {
        ptaskQueue->pFront = ptaskQueue->pRear = pNew;
    }
    else
    {
        ptaskQueue->pRear->pnext = pNew;
        ptaskQueue->pRear = pNew;
    }
    ++ptaskQueue->queueSize;
    return 0;
}

int deQueue(taskQueue_t *ptaskQueue)
{
    if (ptaskQueue->queueSize == 0)
    {
        return 1;
    }
    node_t *tmp = ptaskQueue->pFront;
    ptaskQueue->pFront = ptaskQueue->pFront->pnext;
    delete tmp;
    --ptaskQueue->queueSize;
    if (ptaskQueue->queueSize == 0)
    {
        ptaskQueue->pFront = ptaskQueue->pRear = nullptr;
    }
    return 0;
}