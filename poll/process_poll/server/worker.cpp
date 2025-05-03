#include "head.h"

int makeWorker(int workerNum, workerData_t *workerArr)
{
    for (int i = 0; i < workerNum; ++i)
    {
        pid_t pid = fork();
        if(pid==0){
            while(1){
                sleep(1);
            }
            /*子进程不会继续创建新的子进程*/
        }
        else{
            workerArr[i].pid=pid;/*父进程调用fork返回子进程pid*/
            workerArr[i].status=FREE;
        }
    }
    return 0;
}