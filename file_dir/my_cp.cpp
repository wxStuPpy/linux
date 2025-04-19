#include<func.h>

#define buf_SIZE 10

int main(int argc,char *argv[]){
    ARGC_CHECK(argc,3);
    int fd1=open(argv[1],O_RDONLY);
    int fd2=open(argv[2],O_WRONLY|O_CREAT,0666);
    ERROR_CHECK(fd1,-1,"open read");
    ERROR_CHECK(fd2,-1,"open write");
    char buf[buf_SIZE]={0};
    while(1){
    memset(buf,0,sizeof(buf));
    ssize_t sret=read(fd1,buf,sizeof(buf));
    ERROR_CHECK(sret,-1,"read");
    if(sret==0){
        break;
    }
    sret=write(fd2,buf,sret);
    ERROR_CHECK(sret,-1,"write");
    }
    close(fd1);
    close(fd2);
}