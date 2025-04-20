#include<func.h>
int main(int argc,char*argv[]){
    ARGC_CHECK(argc,2);
    int fd=open(argv[1],O_RDWR);
    ftruncate(fd,10);
    char *p=(char*)mmap(nullptr,10,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,MAP_FAILED,"mmap");
    //将磁盘文件映射到内存中 内存支持随机访问
    for(int i=0;i<10;++i){
        printf("%c",p[i]);
    }
    p[4]='c';
    printf("\n");
    printf("%c",p[4]);
    printf("\n");
    //回收资源 后进先出 
    munmap(p,10);
    close(fd);
    return 0;
}