#include<func.h>

void *fun(void *arg){
    while(1){
        /*如果不打印这行话 那就不存在取消点 pthread_cancel不能结束该线程*/
        printf("I still alive\n");
    }
    pthread_exit(nullptr);
}
/*多线程和信号机制存在冲突*/

//不会直接杀死该线程 而是执行到取消点时才会结束线程
/*取消点 man 7 pthreads
①引发阻塞的 select wait pause...
②文件 open close write read ...
③输入输出 printf scanf
*/
int main(){
    pthread_t tid;
    pthread_create(&tid,nullptr,fun,nullptr);
    usleep(1);
    pthread_cancel(tid);
    pthread_join(tid,nullptr);
    return 0;
}