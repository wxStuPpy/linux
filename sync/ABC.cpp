#include<func.h>

typedef struct sharedRes_s{
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
}sharedRes_t;

void *AC(void*arg){
    sharedRes_t *pRes=(sharedRes_t*)arg;
    pthread_mutex_lock(&pRes->mutex);
    while(pRes->flag!=1){
        pthread_cond_wait(&pRes->cv,&pRes->mutex);
    }
    printf("A\n");
    sleep(1);
    pRes->flag=2;
    pthread_cond_broadcast(&pRes->cv);
    pthread_mutex_unlock(&pRes->mutex);

    pthread_mutex_lock(&pRes->mutex);
    while(pRes->flag!=3){
        pthread_cond_wait(&pRes->cv,&pRes->mutex);
    }
    printf("C\n");
    sleep(1);
    pthread_mutex_unlock(&pRes->mutex);

    pthread_exit(nullptr);
}



int main(void){
    sharedRes_t res;
    res.flag=0;//1 A 2 B 3 C
    pthread_mutex_init(&res.mutex,nullptr);
    pthread_cond_init(&res.cv,nullptr);

    pthread_t tid1;
    pthread_create(&tid1,nullptr,AC,&res);

    pthread_mutex_lock(&res.mutex);
    res.flag=1;
    pthread_cond_broadcast(&res.cv);
    pthread_mutex_unlock(&res.mutex);

    pthread_mutex_lock(&res.mutex);
    while(res.flag!=2){
    pthread_cond_wait(&res.cv,&res.mutex);
    }
    printf("B\n");
    sleep(1);
    res.flag=3;
    pthread_cond_broadcast(&res.cv);
    pthread_mutex_unlock(&res.mutex);

    pthread_join(tid1,nullptr);
  
    return 0;
}