#include <func.h>
#define MAXEVENTS 20
int main(int argc, char *argv[])
{

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret=connect(sockfd,(sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");
    int epfd = epoll_create(1);

    /*监听集合*/
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);
    ERROR_CHECK(ret, -1, "epoll_ctl stdin");

    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    ERROR_CHECK(ret, -1, "epoll_ctl sockfd");
    char buf[4096] = {0};
    while (1)
    {
        /*就绪集合*/
        struct epoll_event readyEvents[MAXEVENTS];
        int readyNum = epoll_wait(epfd, readyEvents, MAXEVENTS, -1);
        for (int i = 0; i < readyNum; ++i)
        {
            if (readyEvents[i].data.fd == STDIN_FILENO)
            {
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(sockfd, buf, strlen(buf), 0);
            }
            else if (readyEvents[i].data.fd == sockfd)
            {
                memset(buf, 0, sizeof(buf));
                ssize_t sret = recv(sockfd, buf, sizeof(buf), 0);
                if(sret==0){
                    close(sockfd);
                    printf("server close connection\n");
                    break;
                }
                printf("buf =%s\n", buf);
            }
        }
    }
    return 0;
}