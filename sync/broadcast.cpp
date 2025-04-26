#include<func.h>

typedef struct sharedRes_s{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
}sharedRes_t;

void* classmate1(void *arg){
    sharedRes_t *pRes=(sharedRes_t*)arg;
    pthread_mutex_lock(&pRes->mutex);
    while(pRes->flag!=1){
        printf("1 is not ready\n");
        pthread_cond_wait(&pRes->cv,&pRes->mutex);
    }
    printf("I got 1\n");
    pRes->flag=0;
    pthread_mutex_unlock(&pRes->mutex);
    pthread_exit(nullptr);
}

void* classmate2(void *arg){
    sharedRes_t *pRes=(sharedRes_t*)arg;
    pthread_mutex_lock(&pRes->mutex);
    while(pRes->flag!=2){
        printf("2 is not ready\n");
        pthread_cond_wait(&pRes->cv,&pRes->mutex);
    }
    printf("I got 2\n");
    pRes->flag=0;
    pthread_mutex_unlock(&pRes->mutex);
    pthread_exit(nullptr);
}

int main(void){
    sharedRes_t res;
    res.flag=0;
    pthread_mutex_init(&res.mutex,nullptr);
    pthread_cond_init(&res.cv,nullptr);

    pthread_t tid1,tid2;
    pthread_create(&tid1,nullptr,classmate1,&res);
    pthread_create(&tid2,nullptr,classmate2,&res);

    while(1){
        int i;
        scanf("%d",&i);
        printf("I producer %d\n",i);
        pthread_mutex_lock(&res.mutex);
        res.flag=i;
        pthread_cond_broadcast(&res.cv);
        pthread_mutex_unlock(&res.mutex);
    }

    pthread_join(tid1,nullptr);
    pthread_join(tid2,nullptr);
    return 0;
}