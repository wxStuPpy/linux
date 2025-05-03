#include"head.h"

int main(int argc,char *argv[]){
    workerData_t workerArr[3];
    makeWorker(3,workerArr);
    for(int i=0;i<3;++i){
        printf("pid = %d status = %d\n",workerArr[i].pid,workerArr[i].status);
    }
    return 0;
}