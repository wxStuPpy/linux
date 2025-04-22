#include<func.h>
/*所有守护进程都是孤儿进程*/
void Daemon(){
    //新建会话
    if(fork()){
        exit(0);
    }
    /*不能直接setsid bash之后的进程默认为进程组组长*/
    setsid();
    //关闭设备
    for(int i=0;i<100;++i){
        close(i);
    }
    //修改环境属性
    chdir("/");
    umask(0);
}

int main(int argc,char *argv[]){
    Daemon();
    for(int i=0;i<20;++i){
        syslog(LOG_INFO,"I am daemon,i = %d",i);
        sleep(1);
    }
    
    return 0;
}