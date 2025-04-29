#include <func.h>

int main(int argc, char *argv[])
{
    ARGC_CHECK(argc, 3);
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");
    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 50);
    ERROR_CHECK(ret, -1, "listen");

    fd_set monitorSet;
    fd_set readySet;
    FD_ZERO(&monitorSet);
    FD_SET(sockfd, &monitorSet);
    char buf[4096] = {0};
    // 设计一个数据结构 存储所有客户端的netfd
    int netfds[1024];
    memset(netfds, -1, sizeof(netfds));
    int curidx = 0;    // 下一次加入的netfd的下标
    int fdtoidx[1024]; // 用来查找fd下标的哈希表
    memset(fdtoidx, -1, sizeof(fdtoidx));
    while (1)
    {
        memcpy(&readySet, &monitorSet, sizeof(monitorSet));
        select(1024, &readySet, nullptr, nullptr, nullptr);
        if (FD_ISSET(sockfd, &readySet))
        {
            netfds[curidx] = accept(sockfd, nullptr, nullptr);
            printf("i =%d, netfd = %d\n", curidx, netfds[curidx]);
            fdtoidx[netfds[curidx]] = curidx;
            FD_SET(netfds[curidx], &monitorSet);
            ++curidx;
        }
        for (int i = 0; i < curidx; ++i)
        {
            if (netfds[i] != -1 && FD_ISSET(netfds[i], &readySet))
            {
                memset(buf, 0, sizeof(buf));
                ssize_t sret = recv(netfds[i], buf, sizeof(buf), 0);
                /*某个客户端断开连接*/
                if (sret == 0)
                {
                    FD_CLR(netfds[i], &monitorSet);
                    fdtoidx[netfds[i]] = -1;
                    close(netfds[i]);
                    printf("client %d break\n", netfds[i]);
                    netfds[i] = -1;
                    continue;
                }
                for (int j = 0; j < curidx; ++j)
                {
                    if (netfds[j] != -1 && j != i)
                    {
                        send(netfds[j], buf, strlen(buf), 0);
                    }
                }
            }
        }
    }

    close(sockfd);
    return 0;
}