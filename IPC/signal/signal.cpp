#include<func.h>

/// @brief 更改信号的行为
/// @param signum 针对哪一个信号
/// @param handler 新的回调函数
/// @return 原来的旧信号 一般不使用
// typedef void (*sighandler_t)(int);
//sighandler_t signal(int signum, sighandler_t handler);

void handle(int signum){
    printf("signum = %d\n",signum);
}
int main(void){
    signal(SIGINT,handle);//signal是不会阻塞的
    while(1){
        sleep(1);
    }
}