#include "head.h"

int makeWorker(int workerNum, workerData_t *workerArr)
{
    for (int i = 0; i < workerNum; ++i)
    {   /*在fork之前创建管道*/
        int pipefd[2];
        socketpair(AF_LOCAL,SOCK_STREAM,0,pipefd);
        pid_t pid = fork();
        if(pid==0){
            close(pipefd[1]);
            workLoop(pipefd[0]);
            /*子进程不会继续创建新的子进程*/
        }
        else{
            close(pipefd[0]);
            workerArr[i].pid=pid;/*父进程调用fork返回子进程pid*/
            workerArr[i].status=FREE;
            workerArr[i].pipesockfd=pipefd[1];
        }
    }
    return 0;
}

int workLoop(int sockfd){
    while(1){
        int netfd;
        recvfd(sockfd,&netfd);/*接受任务*/
        printf("begin work\n");
        sleep(2);
        printf("after work\n");
        /*子进程完成任务 发送自己的pid告诉父进程已经完成任务*/
        pid_t pid=getpid();
        send(sockfd,&pid,sizeof(pid),0);
    }
}