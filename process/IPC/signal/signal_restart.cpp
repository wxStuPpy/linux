#include<func.h>
void handle(int signum){
    printf("signum = %d\n",signum);
}
//自动重启低速系统调用 read(stdin fdr net)
int main(void){
    signal(SIGINT,handle);
    char buf[1024];
    //收到信号后 会重新启动read
    ssize_t sret=read(STDIN_FILENO,buf,sizeof(buf));
    printf("sret = %ld buf = %s\n",sret,buf);
    return 0;
}