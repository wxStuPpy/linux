#include<func.h>
/*主进程使用 保存每个工作进程的信息*/
enum Status{
    FREE,
    BUSY
};

typedef struct workerData_s{
    pid_t pid;
    Status status;
}workerData_t;

int makeWorker(int workerNum,workerData_t *workerArr);