#include<func.h>
int main(int argc,char *argv[]){
    ARGC_CHECK(argc,3);
    int fdw=open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open write");
    int fdr=open(argv[2],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open read");

    char buf[4096];
    fd_set rdset;

    //  记录select返回的时间
    time_t curTime;
    //最后一次活跃时间
    time_t lastActive=time(nullptr);

    while(1){
    FD_ZERO(&rdset);
    FD_SET(STDIN_FILENO,&rdset);
    FD_SET(fdr,&rdset);
    // 设置超时
    timeval timeout={1,0};
    int ret=select(fdr+1,&rdset,nullptr,nullptr,&timeout);
    //更新当前时间
    curTime=time(nullptr);
    printf("curTime = %s\n",ctime(&curTime));
    if(FD_ISSET(fdr,&rdset)){
        //更新最后一次活跃时间
        lastActive=time(nullptr);
        printf("lastActive = %s\n",ctime(&lastActive));
        memset(buf,0,sizeof(buf));
        ssize_t sret=read(fdr,buf,sizeof(buf));
        if(sret==0){
            break;
        }
        printf("buf=%s\n",buf);
    }
    if(FD_ISSET(STDIN_FILENO,&rdset)){
        memset(buf,0,sizeof(buf));
        ssize_t sret=read(STDIN_FILENO,buf,sizeof(buf));
        if(sret==0){
            write(fdw,"bye b",strlen("bye a"));
            break;
        }
        write(fdw,buf,strlen(buf));
    }
    if(curTime-lastActive>=10){
        printf("byebye!\n");
        break;
    }
    }
    close(fdr);
    close(fdw);
    return 0;
}