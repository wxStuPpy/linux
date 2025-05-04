#include"worker.h"
#include"taskQueue.h"
#include"threadPool.h"
#include"tcpInit.h"
#include"epoll.h"

int exitPipe[2];
void handler(int signum){
    printf("signum = %d\n",signum);
    write(exitPipe[1],"1",1);
}

int main(int argc,char *argv[]){
    ARGC_CHECK(argc,4);
    pipe(exitPipe);
    if(fork()!=0){
        close(exitPipe[0]);
        signal(SIGUSR1,handler);
        wait(nullptr);
        printf("parent is going to exit\n");
        exit(0);
    }
  
    /*只有子进程才能创建线程池*/
    close(exitPipe[1]);
    threadPool_t threadPool;
    threadPoolInit(&threadPool,atoi(argv[3]));
    makeWorker(&threadPool);
    int sockfd;
    tcpInit(argv[1],argv[2],&sockfd);

    int epfd=epoll_create(1);
    epollAdd(epfd,sockfd);
    epollAdd(epfd,exitPipe[0]);
    while(1){
       struct epoll_event readySet[1024];
       int readyNum=epoll_wait(epfd,readySet,1024,-1);
       for(int i=0;i<readyNum;++i){
        if(readySet[i].data.fd==sockfd){
            int netfd=accept(sockfd,nullptr,nullptr);
            printf("New connection accepted, netfd = %d\n", netfd);
            ERROR_CHECK(netfd,-1,"accept");
            /*分配任务*/
            pthread_mutex_lock(&threadPool.mutex);
            enQueue(&threadPool.taskQueue,netfd);
            printf("I am master, I send a netfd %d\n",netfd);
            pthread_cond_signal(&threadPool.cond);
            pthread_mutex_unlock(&threadPool.mutex);
        }
        /*父进程结束子进程 子进程结束线程池 多线程不能使用信号机制*/
        else{
            printf("threadPool is going to exit\n");
            /*cancel问题太多 使用标志位带结束子线程*/
            // for(int j=0;j<threadPool.tidArr.workerNum;++j){
            //     /*子线程可能在wait处带着锁终止 导致死锁*/
            //     pthread_cancel(threadPool.tidArr.arr[j]);
            // }
            pthread_mutex_lock(&threadPool.mutex);
            threadPool.exitFlag=1;
            pthread_cond_broadcast(&threadPool.cond);
            pthread_mutex_unlock(&threadPool.mutex);
            for(int j=0;j<threadPool.tidArr.workerNum;++j){
                pthread_join(threadPool.tidArr.arr[j],nullptr);
            }
            printf("main thread is going to exit\n");
            exit(0);
        }
       } 
    }
    return 0;
}