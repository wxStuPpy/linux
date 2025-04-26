#include <func.h>

typedef struct sharedRes_s
{
    int ticketNum;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
} sharedRes_t;

#if 0

void *sellWindows1(void *arg){
    sharedRes_t*pRes=(sharedRes_t*)arg;
    /*error 当num=1时 可能读到num>0时 切走到其他线程 将num减为0 再切换到该线程将num减为-1 */
    while(pRes->ticketNum>0){
        pthread_mutex_lock(&pRes->mutex);
        --pRes->ticketNum;
        printf("sellWindows1 tickNum = %d\n",pRes->ticketNum);
        pthread_mutex_unlock(&pRes->mutex);
    }
    pthread_exit(nullptr);
}

#endif

void *sellWindows1(void *arg)
{
    sharedRes_t *pRes = (sharedRes_t *)arg;
    while (1)
    {
        // 读共享变量的时候 也必须加锁
        pthread_mutex_lock(&pRes->mutex);
        if (pRes->ticketNum > 0)
        {
            --pRes->ticketNum;
            printf("sellWindows1 tickNum = %d\n", pRes->ticketNum);
            if (pRes->ticketNum < 10)
            {
                pthread_cond_signal(&pRes->cv);
            }
        }
        else
        {
            pthread_mutex_unlock(&pRes->mutex);
            break;
        }
        pthread_mutex_unlock(&pRes->mutex);
        usleep(100);
    }
    pthread_exit(nullptr);
}

void *sellWindows2(void *arg)
{
    sharedRes_t *pRes = (sharedRes_t *)arg;
    while (1)
    {
        pthread_mutex_lock(&pRes->mutex);
        if (pRes->ticketNum > 0)
        {
            --pRes->ticketNum;
            printf("sellWindows2 tickNum = %d\n", pRes->ticketNum);
            if (pRes->ticketNum < 10)
            {

                pthread_cond_signal(&pRes->cv);
            }
        }
        else
        {
            pthread_mutex_unlock(&pRes->mutex);
            break;
        }
        pthread_mutex_unlock(&pRes->mutex);
        usleep(100);
    }
    pthread_exit(nullptr);
}

void *addTicket(void *arg)
{
    sharedRes_t *pRes = (sharedRes_t *)arg;
    pthread_mutex_lock(&pRes->mutex);
    if (pRes->ticketNum >= 10)
    {
        pthread_cond_wait(&pRes->cv, &pRes->mutex);
    }
    pRes->ticketNum += 10;
    printf("after addTick tickNum = %d\n", pRes->ticketNum);
    pthread_mutex_unlock(&pRes->mutex);
    pthread_exit(nullptr);
}

int main(void)
{
    sharedRes_t res;
    res.ticketNum = 20;
    pthread_mutex_init(&res.mutex, nullptr);
    pthread_cond_init(&res.cv, nullptr);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, nullptr, sellWindows1, &res);
    pthread_create(&tid2, nullptr, sellWindows2, &res);
    pthread_create(&tid3, nullptr, addTicket, &res);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
    pthread_join(tid3, nullptr);

    return 0;
}