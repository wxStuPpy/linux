#include"head.h"

int sendfd(int sockfd, int fdtosend,int exitFlag)
{
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(hdr)); /*不可省略 name-->nullptr namelen-->0*/
    /*无论正文是否有用 都必修要有正文*/
    /*iovec数组可以发送完整的一份离散的内容 例如链表*/
    struct iovec vec[1];
    vec[0].iov_base = &exitFlag;
    vec[0].iov_len = sizeof(exitFlag);
    hdr.msg_iov = vec;
    hdr.msg_iovlen = 1;
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

int recvfd(int sockfd, int *pfdtorecv,int *pexitFlag)
{
    struct msghdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    struct iovec vec[1];
    vec[0].iov_base = pexitFlag;
    vec[0].iov_len = sizeof(int);
    hdr.msg_iov = vec;
    hdr.msg_iovlen = 1;
    struct cmsghdr *pcmsg = new cmsghdr[CMSG_LEN(sizeof(int))];
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS;
    hdr.msg_control = pcmsg;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = recvmsg(sockfd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    *pfdtorecv = *(int *)CMSG_DATA(pcmsg);
    //printf("buf1 = %s, buf2 = %s, fd = %d\n", buf1, buf2, *(int *)CMSG_DATA(pcmsg));
    return 0;
}