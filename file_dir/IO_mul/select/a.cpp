#include<func.h>
int main(int argc,char *argv[]){
    ARGC_CHECK(argc,3);
    int fdr=open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open read");
    int fdw=open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open write");
    char buf[4096];

    //为fd_set 申请内存空间
    fd_set rdset;
    while(1){
    //清零 //开始是监听集合
    FD_ZERO(&rdset);
    //添加需要监听的文件描述符
    FD_SET(STDIN_FILENO,&rdset);
    FD_SET(fdr,&rdset);
    // 设置超时
    timeval timeout={3,0};
    if(select(fdr+1,&rdset,nullptr,nullptr,&timeout)==0){
        printf("timeout\n");
        continue;
    }
    //select返回 说明fdr或stdin就绪 rdset是就绪集合
    if(FD_ISSET(fdr,&rdset)){
        // 读取b的消息
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
            write(fdw,"bye a",strlen("bye a"));
            break;
        }
        write(fdw,buf,strlen(buf));
    }
    }
    close(fdw);
    close(fdr);
    return 0;
}