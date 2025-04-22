#include<func.h>
void handle(int signum){
    printf("begin signum = %d\n",signum);
    sleep(1);
    printf("after signum = %d\n",signum);
}
int main(void){
    signal(SIGINT,handle);
    signal(SIGQUIT,handle);
    while(1){
        sleep(1);
    }
}