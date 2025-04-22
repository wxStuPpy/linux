#include<func.h>
void handle(int signum){
    printf("begin signum = %d\n",signum);
    sleep(1);
    printf("after signum = %d\n",signum);
}
int main(void){
    signal(SIGINT,handle);//signal是不会阻塞的 存在信号屏蔽 必须当前信号递送完毕
    while(1){
        sleep(1);
    }
}