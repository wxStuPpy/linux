#include<func.h>
int main(int argc,char*argv[]){
    int fds1[2],fds2[2];
    pipe(fds1);
    pipe(fds2);
    //0D 1W
    if(fork()){
        //父进程利用fds1的写端 fds2的读端
        close(fds1[0]);
        close(fds2[1]);
        const char *message="receive a message form father";
        write(fds1[1],message,strlen(message)+1);
        char readBuf[1024];
        read(fds2[0],readBuf,sizeof(readBuf));
        printf("father readbuf = %s\n",readBuf);
        close(fds1[1]);
        close(fds2[0]);
        wait(nullptr);
    }
    else{
        // 子进程利用fds1的读端 fds2的写端
        close(fds1[1]);
        close(fds2[0]);
        const char *message="receive a message form son";
        write(fds2[1],message,strlen(message)+1);
        char readBuf[1024];
        read(fds1[0],readBuf,sizeof(readBuf));
        printf("son readbuf = %s\n",readBuf);
        close(fds1[0]);
        close(fds2[1]);
    }
    return 0;
}