#include<func.h>
#define NUM 10000000
typedef struct shared_Res_s{
    int val;
    pthread_mutex_t mutex;//锁的临界区域越小越好 防止出现饥饿问题
}shared_Res_t;

void* fun(void *arg){
    shared_Res_t *p=(shared_Res_t*)arg;
    for(int i=0;i<NUM;++i){
        pthread_mutex_lock(&p->mutex);
        ++p->val;
        pthread_mutex_unlock(&p->mutex);
    }
    pthread_exit(nullptr);
}

int main(void){
    shared_Res_t sr;
    sr.val=0;
    pthread_mutex_init(&sr.mutex,nullptr);
    pthread_t tid;
    //测试加锁时间
    struct timeval beginTime,endTime;
    gettimeofday(&beginTime,nullptr);
    pthread_create(&tid,nullptr,fun,&sr);
    for(int i=0;i<NUM;++i){
        pthread_mutex_lock(&sr.mutex);
        ++sr.val;
        pthread_mutex_unlock(&sr.mutex);
    }
    pthread_join(tid,nullptr);
    gettimeofday(&endTime,nullptr);
    printf("duration = %ldus val = %d\n",
        ((endTime.tv_sec-beginTime.tv_sec)*1000*1000+endTime.tv_usec-beginTime.tv_usec),sr.val);
    return 0;
}