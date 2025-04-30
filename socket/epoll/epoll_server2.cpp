#include <func.h>

typedef struct Conn_s
{
    bool isConnected;
    int netfd;
    time_t lastActive;
} Conn_t;

#define MAXEVENTS 20

int main(int argc, char *argv[])
{
    ARGC_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &serverAddr.sin_addr);

    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 40);

    int epfd = epoll_create(1);

    /*先加入监听fd*/
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
    ERROR_CHECK(ret, -1, "epoll_ctl server");

    int curidx = 0;
    int netfd = -1;
    Conn_t conns[MAXEVENTS];
    memset(conns, 0, sizeof(conns));
    int fdtoidx[MAXEVENTS];
    memset(fdtoidx, -1, sizeof(fdtoidx));
    char buf[4096] = {0};
    /*开始轮询*/
    while (1)
    {
        struct epoll_event readyEvents[MAXEVENTS];
        int readyNum = epoll_wait(epfd, readyEvents, MAXEVENTS, 1000);
        time_t now = time(nullptr);
        for (int i = 0; i < readyNum; ++i)
        {
            int fd = readyEvents[i].data.fd;
            /*有新的连接*/
            if (fd == sockfd)
            {
                /*加入新的连接到数组中*/
                netfd = accept(sockfd, nullptr, nullptr);
                printf("connected netfd = %d curidx = %d\n", netfd, curidx);
                conns[curidx].isConnected = true;
                conns[curidx].netfd = netfd;
                conns[curidx].lastActive = time(nullptr);
                fdtoidx[netfd] = curidx++;
                /*加入新的fd到epoll树中*/
                ev.data.fd = netfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, &ev);
            }
            /*群发*/
            else
            {
                memset(buf, 0, sizeof(buf));
                ssize_t sret = recv(fd, buf, sizeof(buf), 0);
                /*客户端自己断开*/
                if (sret == 0)
                {
                    printf("client netfd %d close connection\n", fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                    close(fd);
                    int idx = fdtoidx[fd];
                    conns[idx].isConnected = false;
                    fdtoidx[fd] = -1; 
                    conns[idx].netfd = -1;
                    continue;
                }
                int idx=fdtoidx[fd];
                conns[idx].lastActive = time(nullptr);
                for (int j = 0; j < curidx; ++j)
                {
                    // 已连接 不等于当前
                    if (conns[j].isConnected == true && conns[j].netfd != fd)
                    {
                        send(conns[j].netfd, buf, strlen(buf), 0);
                    }
                }
            }
        }
        /*踢人*/
        for (int i = 0; i < curidx; ++i)
        {
            if (conns[i].isConnected &&
                now - conns[i].lastActive > 10)
            {
                printf("kick netfd %d\n", conns[i].netfd);
                epoll_ctl(epfd, EPOLL_CTL_DEL, conns[i].netfd, nullptr);
                close(conns[i].netfd);
                conns[i].isConnected = false;
                fdtoidx[conns[i].netfd] = -1;
                conns[i].netfd = -1;
            }
        }
        // 连接数控制
        if (curidx >= MAXEVENTS)
        {
            printf("Max connections reached\n");
            close(netfd);
            close(sockfd);
            close(epfd);
            exit(0);
        }
    }
}
