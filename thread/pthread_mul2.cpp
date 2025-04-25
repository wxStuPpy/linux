#include<func.h>

void *fun(void *arg){
   long p=(long)arg;
    printf("*p = %ld\n",p);
    return nullptr;
}

int main(void){
    pthread_t tid1,tid2,tid3;
    long i=1000;
    pthread_create(&tid1,nullptr,fun,(void*)i);
    ++i;
    pthread_create(&tid2,nullptr,fun,(void*)i);
    ++i;
    pthread_create(&tid3,nullptr,fun,(void*)i);
    sleep(1);
    return 0;
}