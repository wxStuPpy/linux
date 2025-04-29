#include <func.h>

typedef struct Conn_s
{
    int isConnected;
    int netfd;
    time_t lastActive;
} Conn_t;

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
    // 设计一个数据结构 存储所有客户端的netfd和上次活跃时间
    Conn_t clients[1024];
    for (int i = 0; i < 1024; ++i)
    {
        clients[i].isConnected = 0;
    }

    int curidx = 0;    // 下一次加入的netfd的下标
    int fdtoidx[1024]; // 用来查找fd下标的哈希表
    memset(fdtoidx, -1, sizeof(fdtoidx));

    time_t now;

    while (1)
    {
        memcpy(&readySet, &monitorSet, sizeof(monitorSet));
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        select(1024, &readySet, nullptr, nullptr, &timeout);
        now = time(nullptr);
        printf("now = %s\n", ctime(&now));

        if (FD_ISSET(sockfd, &readySet))
        {
            clients[curidx].netfd = accept(sockfd, nullptr, nullptr);
            clients[curidx].isConnected = 1;
            clients[curidx].lastActive = time(nullptr);
            printf("curidx =%d, netfd = %d, now =%s\n", curidx, clients[curidx].netfd, ctime(&clients[curidx].lastActive));
            fdtoidx[clients[curidx].netfd] = curidx;
            FD_SET(clients[curidx].netfd, &monitorSet);
            ++curidx;
        }
        for (int i = 0; i < curidx; ++i)
        {
            if (clients[i].isConnected != 0 && FD_ISSET(clients[i].netfd, &readySet))
            {
                memset(buf, 0, sizeof(buf));
                ssize_t sret = recv(clients[i].netfd, buf, sizeof(buf), 0);
                clients[i].lastActive = time(nullptr);
                printf("i =%d, netfd = %d, now =%s\n", i, clients[i].netfd, ctime(&clients[i].lastActive));
                /*某个客户端断开连接*/
                if (sret == 0)
                {
                    FD_CLR(clients[i].netfd, &monitorSet);
                    fdtoidx[clients[i].netfd] = -1;
                    close(clients[i].netfd);
                    printf("client %d break\n", clients[i].netfd);
                    clients[i].isConnected = 0;
                    continue;
                }
                for (int j = 0; j < curidx; ++j)
                {
                    if (clients[j].netfd != 0 && j != i)
                    {
                        send(clients[j].netfd, buf, strlen(buf), 0);
                    }
                }
            }
        }
        for (int i = 0; i < curidx; ++i)
        {
            if (clients[i].isConnected == 1 && now - clients[i].lastActive > 5)
            {
                FD_CLR(clients[i].netfd, &monitorSet);
                fdtoidx[clients[i].netfd] = -1;
                close(clients[i].netfd);
                printf("client %d kick\n", clients[i].netfd);
                clients[i].isConnected = 0;
            }
        }
    }

    close(sockfd);
    return 0;
}