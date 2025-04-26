#include<func.h>

bool flag=false;

typedef struct sharedRes_s{
    int val;
    pthread_mutex_t mutex;
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
    /*空转 占用cpu*/
    while(1){
        pthread_mutex_lock(&pRes->mutex);
        if(flag==true){
        pthread_mutex_unlock(&pRes->mutex);
            break;
        }
        pthread_mutex_unlock(&pRes->mutex);
    }
    B();
    pthread_exit(nullptr);
}

int main(void){
    sharedRes_t res;
    res.val=0;
    pthread_mutex_init(&res.mutex,nullptr);

    pthread_t tid;
    pthread_create(&tid,nullptr,fun,&res);

    A();
    pthread_mutex_lock(&res.mutex);
    flag=true;
    pthread_mutex_unlock(&res.mutex);

    pthread_join(tid,nullptr);
    return 0;
}