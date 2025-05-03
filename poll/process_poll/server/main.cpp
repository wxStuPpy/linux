#include"head.h"

int main(int argc,char *argv[]){
    ARGC_CHECK(argc,4);
    int workerNum=atoi(argv[3]);
    /*创建子进程*/
    workerData_t *workerArr=new workerData_t[workerNum];
    makeWorker(workerNum,workerArr);
    /*初始化tcp服务端*/
    int sockfd;
    tcpInit(argv[1],argv[2],&sockfd);
    /*初始化epoll*/
    int epfd=epoll_create(1);
    epollAdd(epfd,sockfd);
    while(1){
        struct epoll_event readSet[1024];
        int readNum=epoll_wait(epfd,readSet,1024,-1);
        for(int i=0;i<readNum;++i){
            if(readSet[i].data.fd==sockfd){
                int netfd=accept(sockfd,nullptr,nullptr);
                printf("netfd = %d connected!\n",netfd);
                // epollAdd(epfd,netfd);
            }
            else{

            }
        }
    }
    return 0;
}