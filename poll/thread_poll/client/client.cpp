#include <func.h>
typedef struct train_s
{
    int len;
    char data[1024];
} train_t;

/*可以替代recv 同时不需要最后一个参数MSG_WAITALL*/
int recvn(int sockfd,void* buf,long total){
    char *p=(char*)buf;
    off_t curSize=0;
    while (curSize<total)
    {
        ssize_t sret=recv(sockfd,p+curSize,total-curSize,0);
        if(sret==0){
            return 1;
        }
        curSize+=sret;
    }
    return 0;
}


// 1.0 存在tcp粘包问题
#if 0
int recvFile(int sockfd){
    /*tcp是流式协议  可能会一次性读出 file1和helloword 必须使用协议确定边界*/
    sleep(1);
    char fileName[4096]={0};
    recv(sockfd,fileName,sizeof(fileName),0);
    int fd=open(fileName,O_CREAT|O_RDWR,0666);
    char buf[1024]={0};
    ssize_t sret=recv(sockfd,buf,sizeof(buf),0);
    write(fd,buf,sret);
    return 0;
}
#elif 0
// 2.0小火车协议 读大文件 存在半包问题 使用MSG_WAITALL来解决
/*利用md5sum或者sha1sum 哈希算法 验证文件内容是否相同  md5sum file1-->bash*/
int recvFile(int sockfd)
{
    char fileName[4096] = {0};
    train_t train;
    /*先收火车头 得到车厢实际长度*/
    recv(sockfd, &train.len, sizeof(train.len), MSG_WAITALL);
    recv(sockfd, train.data, train.len, MSG_WAITALL);
    memcpy(fileName, train.data, train.len);

    int fd = open(fileName, O_CREAT | O_RDWR | O_TRUNC, 0666);
    while (1)
    {
        recv(sockfd, &train.len, sizeof(train.len), MSG_WAITALL);
        if (train.len == 0)
        {
            break;
        }
        recv(sockfd, train.data, train.len, 0);
        write(fd, train.data, train.len);
    }
    close(fd);
    return 0;
}
#elif 0
// 3.0增加显示进度条功能 兼容4.0
int recvFile(int sockfd)
{
    char fileName[4096] = {0};
    train_t train;

    recvn(sockfd, &train.len, sizeof(train.len));
    recvn(sockfd, train.data, train.len);
    memcpy(fileName, train.data, train.len);

    off_t fileSize;
    printf("fileSize = %ld\n",fileSize);
    recvn(sockfd,&train.len,sizeof(train.len));
    recvn(sockfd,train.data,train.len);
    memcpy(&fileSize,train.data,train.len);

    int fd = open(fileName, O_CREAT | O_RDWR | O_TRUNC, 0666);

    off_t curSize=0;

    while (1)
    {
        recvn(sockfd, &train.len, sizeof(train.len));
        if (train.len == 0)
        {
            break;
        }
        recvn(sockfd, train.data, train.len);
        write(fd, train.data, train.len);
        printf("%5.2lf%%\r",curSize*100.0/fileSize);
        fflush(stdout);
        curSize+=train.len;
    }
    printf("100.00%%\n");
    close(fd);
    return 0;
}

#else
//5.0大火车一次性收完
int recvFile(int sockfd){
    char fileName[4096] = {0};
    train_t train;

    recvn(sockfd, &train.len, sizeof(train.len));
    recvn(sockfd, train.data, train.len);
    memcpy(fileName, train.data, train.len);

    off_t fileSize;
    printf("fileSize = %ld\n",fileSize);
    recvn(sockfd,&train.len,sizeof(train.len));
    recvn(sockfd,train.data,train.len);
    memcpy(&fileSize,train.data,train.len);

    int fd = open(fileName, O_CREAT | O_RDWR | O_TRUNC, 0666);
    ftruncate(fd,fileSize);
    char *p=(char *)mmap(nullptr,fileSize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,MAP_FAILED,"mmap");
    recvn(sockfd,p,fileSize);
    munmap(p,fileSize);
    close(fd);
    return 0;
}

#endif
int main(int argc, char *argv[])
{
    ARGC_CHECK(argc, 3);
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int ret = connect(sockfd, (sockaddr *)&serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "connect");
    sleep(10);
    recvFile(sockfd);
    close(sockfd);
    return 0;
}