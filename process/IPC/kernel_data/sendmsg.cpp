#include <func.h>

int sendfd(int sockfd, int fdtosend)
{
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(hdr)); /*不可省略 name-->nullptr namelen-->0*/
    /*无论正文是否有用 都必修要有正文*/
    char buf1[] = "hello";
    char buf2[] = "world";
    /*iovec数组可以发送完整的一份离散的内容 例如链表*/
    struct iovec vec[2];
    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 5;
    hdr.msg_iov = vec;
    hdr.msg_iovlen = 2;
    /*控制字段 记录要发送的内核数据内容 此处发送一个文件描述符 必须建立在堆上*/
    struct cmsghdr *pcmsg = new cmsghdr[CMSG_LEN(sizeof(int))];
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS;
    /*得到data首地址 强转为int* 解引用 */
    *(int *)CMSG_DATA(pcmsg) = fdtosend;
    hdr.msg_control = pcmsg; /*将控制字段的信息放入到hdr中*/
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = sendmsg(sockfd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    return 0;
}

int recvfd(int sockfd, int *pfdtorecv)
{
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    char buf1[6] = {0};
    char buf2[6] = {0};
    struct iovec vec[2];
    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 5;
    hdr.msg_iov = vec;
    hdr.msg_iovlen = 2;
    struct cmsghdr *pcmsg = new cmsghdr[CMSG_LEN(sizeof(int))];
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS;
    hdr.msg_control = pcmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = recvmsg(sockfd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    *pfdtorecv = *(int *)CMSG_DATA(pcmsg);
    printf("buf1 = %s, buf2 = %s, fd = %d\n", buf1, buf2, *(int *)CMSG_DATA(pcmsg));
    return 0;
}

int main()
{
    int pipefd[2]; // pipefd[0] 读端, pipefd[1] 写端
    socketpair(AF_LOCAL, SOCK_STREAM, 0, pipefd);

    // 2. 创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        close(pipefd[1]);
        int fdfile;
        recvfd(pipefd[0], &fdfile);
        printf("child fdfile = %d\n",fdfile);
        ssize_t sret=write(fdfile,"world",5);
        printf("sret = %ld\n",sret);
        close(pipefd[0]);
    }
    else
    {
        close(pipefd[0]);
        int fdfile = open("./file", O_RDWR);
        write(fdfile, "hello", 5);
        sendfd(pipefd[1], fdfile);
        printf("father fdfile = %d\n",fdfile);
        wait(0);
    }
    return 0;
}