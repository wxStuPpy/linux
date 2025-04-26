#include<func.h>

typedef struct sharedRes_s{
    int val;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
    bool flag;
}sharedRes_t;

void A(){
    printf("before A\n");
    sleep(1);
    printf("after A\n");
}

void B(){
    printf("before B\n");
    sleep(1);
    printf("after B\n");
}

void* fun(void *arg){
    sharedRes_t *pRes=(sharedRes_t*)arg;
    pthread_mutex_lock(&pRes->mutex);
    if(pRes->flag==false){
        //检查是否锁上mutex->移入唤醒队列->解锁并等待
        pthread_cond_wait(&pRes->cv,&pRes->mutex);
        //醒来->加锁->带着锁返回
    }
    pthread_mutex_unlock(&pRes->mutex);
    B();
    pthread_exit(nullptr);
}

int main(void){
    sharedRes_t res;
    res.val=0;
    res.flag=false;
    pthread_mutex_init(&res.mutex,nullptr);
    pthread_cond_init(&res.cv,nullptr);

    pthread_t tid;
    pthread_create(&tid,nullptr,fun,&res);

    A();
    pthread_mutex_lock(&res.mutex);
    res.flag=true;
    pthread_cond_signal(&res.cv);
    pthread_mutex_unlock(&res.mutex);

    pthread_join(tid,nullptr);
    return 0;
}