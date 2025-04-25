#include<func.h>

void *fun(void *arg){
    int *p=(int*)arg;
    printf("*p = %d\n",*p);
    return nullptr;
}

void createThread(){
    int i=1000;
    pthread_t tid;
    pthread_create(&tid,nullptr,fun,&i);
    //如果不sleep 可能子线程访问栈空间之前 该函数执行完
    //就销毁了栈 出现栈错误
    sleep(1);
}

int main(void){
    createThread();
    sleep(2);
    return 0;
}