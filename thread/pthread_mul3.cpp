#include<func.h>

void *fun(void *arg){
    int *p=(int*)arg;
    printf("*p = %d\n",*p);
    return nullptr;
}

void threadCreate(){
    int i=1000;
    pthread_t tid;
    pthread_create(&tid,nullptr,fun,&i);
    //如果不加上sleep 可能fun函数拿到i的地址时 i已经销毁 会发生栈错误
   sleep(1);
}

int main(void){
    threadCreate();
    sleep(1);
    return 0;
}