#include <func.h>

#define MAXEVENTS 20

int main(int argc, char *argv[])
{
    ARGC_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    ret = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "connect");

    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create client");
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
    char buf[4096] = {0};
    while (1)
    {
        struct epoll_event readyEvents[MAXEVENTS];
        int readyNum = epoll_wait(epfd, readyEvents, MAXEVENTS, -1);
        for (int i = 0; i < readyNum; ++i)
        {      
            if (readyEvents[i].data.fd == sockfd)
            {
                memset(buf, 0, sizeof(buf));
                ssize_t sret = recv(sockfd, buf, sizeof(buf), 0);
                if (sret == 0)
                {
                    printf("server close connected\n");
                    exit(0);
                }
                printf("sret = %ld,buf = %s\n",sret,buf);
            }
            else if (readyEvents[i].data.fd == STDIN_FILENO)
            {
                memset(buf, 0, sizeof(buf));
                ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
                if(sret==0){
                   continue;
                }
                send(sockfd, buf, strlen(buf), 0);
            }
        }
    }
}