#include<func.h>

void *fun(void *arg){
    int *p=(int*)arg;
    printf("*p = %d\n",*p);
    return nullptr;
}

int main(void){
    pthread_t tid1,tid2,tid3;
    int i=1000;
    pthread_create(&tid1,nullptr,fun,&i);
    ++i;
    pthread_create(&tid2,nullptr,fun,&i);
    ++i;
    pthread_create(&tid3,nullptr,fun,&i);
    sleep(1);
    return 0;
}