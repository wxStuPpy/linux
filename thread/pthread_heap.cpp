#include<func.h>
void* routine(void *arg){
    int *p=(int*)arg;
    (*p)++;
    printf("child *p = %d\n",*p);
    return nullptr;
}
int main(void){
    int *p=new int (100);
    pthread_t tid;
    //利用pthread_create第四个参数该共享堆空间
    int ret=pthread_create(&tid,nullptr,routine,p);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    sleep(1);
    printf("main *p = %d\n",*p);
    return 0;
}