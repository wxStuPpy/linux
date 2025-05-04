#include "threadPool.h"

typedef struct train_s{
    int len;//火车头
    char data[1024];/*不表示字符串 保存任何数据 二进制文件等等
    1024不是火车车厢实际大小 是上限*/
}train_t;

//1.0 //存在tcp粘包问题
#if 0 
int transFile(int netfd){
    char fileName[]="file1";
    send(netfd,fileName,5,0);
    int fd=open(fileName,O_RDWR);
    char buf[1024]={0};
    ssize_t sret=read(fd,buf,sizeof(buf));
    /*不能使用strlen 可能是二进制文件*/
    send(netfd,buf,sret,0);
    return 0;
}
#elif 0

//2.0小火车协议 读大文件 存在半包问题 使用MSG_WAITALL来解决
/*利用md5sum或者sha1sum 哈希算法 验证文件内容是否相同  md5sum file1-->bash*/
int transFile(int netfd){
    train_t train;
    char fileName[]="file1";
    train.len=strlen(fileName);
    memcpy(train.data,fileName,train.len);
    /*发送小火车 车厢长度+头的长度*/
    /*MSG_NOSIGNAL不产生SIGPIPE信号 防止子进程崩溃导致父子进程间的管道一直就绪*/
    send(netfd,&train,train.len+sizeof(train.len),MSG_NOSIGNAL);

    int fd=open(fileName,O_RDWR);
    while (1)
    {
        ssize_t sret = read(fd, train.data, sizeof(train.data));
        train.len = sret;
        ssize_t sret1=send(netfd, &train, train.len + sizeof(train.len), MSG_NOSIGNAL);
        /*发送空火车 或者发送失败作为结束标志*/
        if(sret==0||sret1==-1){
            break;
        }
    }
    close(fd);
    return 0;
}
#elif 0

//3.0增加显示进度条功能
int transFile(int netfd){
    train_t train;
    char fileName[]="file1";
    train.len=strlen(fileName);
    memcpy(train.data,fileName,train.len);
    send(netfd,&train,train.len+sizeof(train.len),MSG_NOSIGNAL);

    int fd=open(fileName,O_RDWR);
    /*发送文件的大小*/
    struct stat statbuf;
    fstat(fd,&statbuf);
    train.len=sizeof(off_t);
    memcpy(train.data,&statbuf.st_size,train.len);
    send(netfd,&train,train.len+sizeof(train.len),MSG_NOSIGNAL);

    while (1)
    {
        ssize_t sret = read(fd, train.data, sizeof(train.data));
        train.len = sret;
        ssize_t sret1=send(netfd, &train, train.len + sizeof(train.len), MSG_NOSIGNAL);
        if(sret==0||sret1==-1){
            break;
        }
    }
    close(fd);
    return 0;
}

#elif 0
//4.0利用mmap实现用户态和内核态之间的零拷贝(性能优化不高)
int transFile(int netfd){
    train_t train;
    char fileName[]="file1";
    train.len=strlen(fileName);
    memcpy(train.data,fileName,train.len);
    send(netfd,&train,train.len+sizeof(train.len),MSG_NOSIGNAL);

    int fd=open(fileName,O_RDWR);
    /*发送文件的大小*/
    struct stat statbuf;
    fstat(fd,&statbuf);
    train.len=sizeof(off_t);
    memcpy(train.data,&statbuf.st_size,train.len);
    send(netfd,&train,train.len+sizeof(train.len),MSG_NOSIGNAL);

    char *p=(char *)mmap(nullptr,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,MAP_FAILED,"mmap");
    off_t curSize=0;
    while(1){
       if(curSize>=statbuf.st_size){
        break;
       }
       if(statbuf.st_size-curSize>=1024){
        train.len=1024;
       }
       else{
        train.len=statbuf.st_size-curSize;
       }
       send(netfd,&train.len,sizeof(train.len),MSG_NOSIGNAL);
       /*每一节车厢都是从映射去发过去*/
       send(netfd,p+curSize,train.len,MSG_NOSIGNAL);
       curSize+=train.len;
    }
    train.len=0;
    send(netfd,&train,train.len+sizeof(train.len),MSG_NOSIGNAL);
    munmap(p,statbuf.st_size);
    close(fd);
    return 0;
}
#else
//5.0大火车一次性发完
int transFile(int netfd){
    train_t train;
    /*发送文件名*/
    char fileName[]="file1";
    train.len=strlen(fileName);
    memcpy(train.data,fileName,train.len);
    send(netfd,&train,train.len+sizeof(train.len),MSG_NOSIGNAL);

    int fd=open(fileName,O_RDWR);
    /*发送文件的大小*/
    struct stat statbuf;
    fstat(fd,&statbuf);
    train.len=sizeof(off_t);
    memcpy(train.data,&statbuf.st_size,train.len);
    send(netfd,&train,train.len+sizeof(train.len),MSG_NOSIGNAL);

    /*char *p=(char *)mmap(nullptr,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,MAP_FAILED,"mmap");
    send(netfd,p,statbuf.st_size,MSG_NOSIGNAL);
    munmap(p,statbuf.st_size);*/
    sendfile(netfd,fd,nullptr,statbuf.st_size);//等价mmap
    close(fd);
    return 0;
}
#endif