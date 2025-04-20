#include<func.h>
int main(int argc,char*argv[]){
    ARGC_CHECK(argc,2);
    //既打开了读端 又打开了写端 所以不会阻塞
    int fd1=open(argv[1],O_RDWR);
    int fd2=open(argv[1],O_RDWR);
    printf("pipe is opened!\n");

    fd_set rdset;
    fd_set wrset;

    int cnt=0;
    char buf[4096]={0};

    /*typedef struct
  {
    __fd_mask fds_bits[1024 / (8 * (int) sizeof (__fd_mask))];

  } fd_set;*/

    while(1){
        FD_ZERO(&rdset);
        FD_ZERO(&wrset);
        FD_SET(fd1,&wrset);
        FD_SET(fd2,&rdset);
        select(fd2+1,&rdset,&wrset,nullptr,nullptr);
        //如果可读
        if(FD_ISSET(fd2,&rdset)){
            printf("read ready, cnt = %d\n",cnt++);
            read(fd2,buf,2048);
        }
        //如果可写 最终变为读两次 写一次
        if(FD_ISSET(fd1,&wrset)){
            printf("write ready, cnt = %d\n",cnt++);
            write(fd1,buf,4096);
        }
        sleep(1);
    }
}