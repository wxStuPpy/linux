#include<func.h>
/*主进程使用 保存每个工作进程的信息*/
enum Status{
    FREE,
    BUSY
};

typedef struct workerData_s{
    pid_t pid;
    Status status;
    int pipesockfd;/*父子进程间通信使用的管道*/
}workerData_t;

int makeWorker(int workerNum,workerData_t *workerArr);

int tcpInit(const char*ip,const char*port,int *psockfd);

int epollAdd(int epfd,int fd);
int epollDel(int epfd,int fd);

/*通过一个管道sockfd 来发送内核的文件对象信息 来实现隔离的进程间操作同一个文件对象的目的*/
int sendfd(int sockfd, int fdtosend);
int recvfd(int sockfd, int *pfdtorecv);

int workLoop(int sockfd);

int transFile(int netfd);