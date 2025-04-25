#include<func.h>

void *fun(void *arg){
    pthread_exit((void*)123);
}

int main(){
    pthread_t tid;
    pthread_create(&tid,nullptr,fun,nullptr);
    void *retVal;
    /*error void **retVal pthread_join(tid,retVal);
    这样是一个野指针*/
    //填nullptr则不获取子线程返回值
    /*int pthread_join(pthread_t thread, void **retval);*/
    pthread_join(tid,&retVal);
    printf("retVal = %ld\n",(long)retVal);
    return 0;
}