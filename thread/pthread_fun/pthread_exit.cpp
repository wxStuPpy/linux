#include<func.h>

void *fun(void *arg){
    //直接退出 该线程直接终止
    //return nullptr;
    //可以在任意位置退出线程
    pthread_exit(nullptr);
}

int main(){
    pthread_t tid;
    pthread_create(&tid,nullptr,fun,nullptr);
    printf("pid = %d\n",getpid());
    while(1){
        sleep(1);
    }
    return 0;
}