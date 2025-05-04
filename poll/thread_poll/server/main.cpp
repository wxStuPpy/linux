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
       epoll_event readySet[1024];
       int readyNum=epoll_wait(epfd,readySet,1024,-1); 
    }
    
    return 0;
}