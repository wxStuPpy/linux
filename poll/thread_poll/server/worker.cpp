#include "worker.h"

int tidArrInit(tidArr_t*ptidArr,int workerNum){
    ptidArr->arr=new pthread_t[workerNum];
    ptidArr->workerNum=workerNum;
    return 0; 
}
void *threadFun(void *arg){
    while(1){
        sleep(1);
    }
    pthread_exit(nullptr);
}