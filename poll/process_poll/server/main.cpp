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
    /*监听每个子进程的pipesockfd*/
    for(int i=0;i<workerNum;++i){
        epollAdd(epfd,workerArr[i].pipesockfd);
    }
    while(1){
        struct epoll_event readSet[1024];
        int readNum=epoll_wait(epfd,readSet,1024,-1);
        for(int i=0;i<readNum;++i){
            if(readSet[i].data.fd==sockfd){
                int netfd=accept(sockfd,nullptr,nullptr);
                printf("netfd = %d connected!\n",netfd);
                // epollAdd(epfd,netfd);
                /*顺序找到一个空闲的子进程 去执行
                (后续可以用一个数据结构维护空闲的一个子进程)*/
                for(int j=0;j<workerNum;++j){
                    if(workerArr[j].status==FREE){
                        sendfd(workerArr[j].pipesockfd,netfd);
                        workerArr[j].status==BUSY;
                        break;
                    }
                }
                /*如果所有子进程都是繁忙 可以选择等待或者拒绝连接*/
                close(netfd);
            }
            /*某个子进程完成任务*/
            else{
                /*使用顺序查找fd对应几号worker进程*/
                for(int j=0;j<workerNum;++j){
                    if(workerArr[j].pipesockfd==readSet[i].data.fd){
                        pid_t pid;
                        recv(workerArr[j].pipesockfd,&pid,sizeof(pid),0);
                        printf("pid = %d end\n",pid);
                        workerArr[j].status=FREE;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}