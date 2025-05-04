#include"worker.h"
#include"taskQueue.h"
#include"threadPool.h"
#include"tcpInit.h"
#include"epoll.h"

int main(int argc,char *argv[]){
    ARGC_CHECK(argc,4);

    threadPool_t threadPool;
    threadPoolInit(&threadPool,atoi(argv[3]));
    makeWorker(&threadPool);
    int sockfd;
    tcpInit(argv[1],argv[2],&sockfd);

    int epfd=epoll_create(1);
    epollAdd(epfd,sockfd);
    while(1){
       struct epoll_event readySet[1024];
       int readyNum=epoll_wait(epfd,readySet,1024,-1);
       for(int i=0;i<readyNum;++i){
        if(readySet[i].data.fd==sockfd){
            int netfd=accept(sockfd,nullptr,nullptr);
            ERROR_CHECK(netfd,-1,"accept");
            /*分配任务*/
            pthread_mutex_lock(&threadPool.mutex);
            enQueue(&threadPool.taskQueue,netfd);
            printf("I am master, I send a netfd %d\n",netfd);
            pthread_cond_signal(&threadPool.cond);
            pthread_mutex_unlock(&threadPool.mutex);
        }
       } 
    }
    return 0;
}