#include<func.h>

/*与dup的区别 dup是多个文件描述符指向同一个文件对象 
本方法则是共享同一个文件对象 主线程和子线程的文件描述符是一样的*/
void *fun(void *arg){
    int *pfd=(int *)arg;
    char buf[1024];
    //也共享了偏移指针
    lseek(*pfd,SEEK_SET,0);
    memset(buf,0,sizeof(buf));
    read(*pfd,buf,sizeof(buf));
    printf("buf = %s\n",buf);
    return nullptr;
}

int main(void){
    pthread_t tid;
    int fd=open("./fd_test",O_RDWR|O_CREAT,0666);
    write(fd,"hello",5);
    pthread_create(&tid,nullptr,fun,&fd);
    sleep(2);
    close(fd);
    return 0;
}

