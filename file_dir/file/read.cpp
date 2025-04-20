#include<func.h>
int main(int argc,char *argv[]){
    ARGC_CHECK(argc,2);
    int fd=open(argv[1],O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    char buf[1024]={0};
    //如果一次性读不完 要把buf清空
    //memset(buf,0,siezof(buf));
    ssize_t sret=read(fd,buf,sizeof(buf));
    printf("sret = %ld, buf = %s\n",sret,buf);
    close(fd);
    //ps:磁盘文件不阻塞(file1) 外部设备会阻塞stdin
}