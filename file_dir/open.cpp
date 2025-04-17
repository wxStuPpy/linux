#include<func.h>
int main(int argc,char *argv[]){
    ARGC_CHECK(argc,2);
    int fd=open(argv[1],O_RDWR|O_CREAT,0666);//O_TRUNC可以清空文件
    ERROR_CHECK(fd,-1,"open");
    printf("%d\n",fd);
}