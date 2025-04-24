#include<func.h>
void* routine(void *){
    printf("I am child thread\n");
    return nullptr;
}
int main(void){
    pthread_t tid;
    //线程不能安全使用全局变量errno 存在并发竞争
    //创建线程会分配栈空间
    int ret=pthread_create(&tid,nullptr,routine,nullptr);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    printf("I am main thread\n");
    usleep(100);
    return 0;
}