#include<func.h>

void *fun(void *arg){
    int *p=(int*)arg;
    printf("*p = %d\n",*p);
    return nullptr;
}

int main(void){
    pthread_t tid1;
    int *p=new int(10);
    pthread_create(&tid1,nullptr,fun,p);
    //可能子线程在访问堆之前 就执行了free操作 从而error
    //可以使用valgrind检查内存错误
    delete p;
    sleep(1);
    return 0;
}