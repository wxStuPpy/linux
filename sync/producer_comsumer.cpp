#include<func.h>

typedef struct node_s
{
    int data;
    struct node_s *pNext;
}node_t;

typedef struct queue_s{
    node_t *pFront;
    node_t *pRear;
    int queueSize;
}queue_t;

typedef struct sharedRes_s{
    queue_t queue;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
}sharedRes_t;

int enQueue(queue_t *pQueue,int data){
    node_t *pNew=new node_t{data,nullptr};
    if(pQueue->pFront==nullptr){
        pQueue->pFront=pNew;
        pQueue->pRear=pNew;
    }
    else{
        pQueue->pRear->pNext=pNew;
        pQueue->pRear=pNew;
    }
    ++pQueue->queueSize;
    return 0;
}

int deQueue(queue_t*pQueue){
    if(pQueue->queueSize<=0){
        return -1;
    }
    node_t *pCur=pQueue->pFront;
    pQueue->pFront=pQueue->pFront->pNext;
    --pQueue->queueSize;
    if(pQueue->queueSize==0){
        pQueue->pFront=pQueue->pRear=nullptr;
    }
    delete pCur;
    return 0;
}

int visitQueue(queue_t *pQueue){
    if(pQueue->queueSize==0){
        return 0;
    }
    node_t *pCur=pQueue->pFront;
    while(pCur){
        printf("%d ",pCur->data);
        pCur=pCur->pNext;
    }
    printf("\n");
    return 0;
}

// 释放队列内存
int freeQueue(queue_t* pQueue) {
    while (deQueue(pQueue) == 0);
    return 0;
}

void *producer(void *arg){
    sharedRes_t *pRes=(sharedRes_t*)arg;
    while(1){
        pthread_mutex_lock(&pRes->mutex);
        while(pRes->queue.queueSize>=10){
            pthread_cond_wait(&pRes->cv,&pRes->mutex);
        }
        //结束等待 开始生产
        int tmp=rand()%1000;
        enQueue(&pRes->queue,tmp);
        printf("tid = %ld enData = %d size= %d\n",pthread_self(),tmp,pRes->queue.queueSize);
        pthread_cond_broadcast(&pRes->cv);
        visitQueue(&pRes->queue);
        pthread_mutex_unlock(&pRes->mutex);
        //必须先解锁 再睡觉 不然三秒内别的线程啥也干不了
        sleep(2);
    }

    pthread_exit(nullptr);
}

void *comsumer(void *arg){
    sharedRes_t *pRes=(sharedRes_t*)arg;
    while(1){
        pthread_mutex_lock(&pRes->mutex);
        while(pRes->queue.queueSize<=0){
            pthread_cond_wait(&pRes->cv,&pRes->mutex);
        }
        //结束等待 开始生产
        int tmp=pRes->queue.pFront->data;
        deQueue(&pRes->queue);
        printf("tid = %ld deData = %d size= %d\n",pthread_self(),tmp,pRes->queue.queueSize);
        pthread_cond_broadcast(&pRes->cv);
        visitQueue(&pRes->queue);
        pthread_mutex_unlock(&pRes->mutex);
        sleep(1);
    }
    pthread_exit(nullptr);
}

int main(void){
  

    sharedRes_t res;
    memset(&res.queue,0,sizeof(res.queue));
    pthread_mutex_init(&res.mutex,nullptr);
    pthread_cond_init(&res.cv,nullptr);

    //插入8个元素
    for(int i=0;i<8;++i){
        enQueue(&res.queue,rand()%1000);
    }


    pthread_t tid1,tid2,tid3,tid4,tid5;
    pthread_create(&tid1,nullptr,producer,&res);
    pthread_create(&tid2,nullptr,producer,&res);
    pthread_create(&tid3,nullptr,producer,&res);
    pthread_create(&tid4,nullptr,comsumer,&res);
    pthread_create(&tid5,nullptr,comsumer,&res);

    pthread_join(tid1,nullptr);
    pthread_join(tid2,nullptr);
    pthread_join(tid3,nullptr);
    pthread_join(tid4,nullptr);
    pthread_join(tid5,nullptr);

    //测试单元
    #if 0
    for(int i=0;i<10;++i){
        enQueue(&myQueue,rand()%1000);
    }
    visitQueue(&myQueue);
    //freeQueue(&myQueue);
    for(int i=0;i<10;++i){
        int front=myQueue.pFront->data;
        deQueue(&myQueue);
        printf("front = %d\n",front);
        visitQueue(&myQueue);
    }
    #endif
    return 0;
}