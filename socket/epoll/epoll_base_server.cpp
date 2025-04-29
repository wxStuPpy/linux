#include "func.h"

int main(int argc, char *argv[]) {
    ARGC_CHECK(argc, 3);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse=1;
    int ret=setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");
    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 50);
    ERROR_CHECK(ret, -1, "listen");
    int netfd = accept(sockfd, NULL, NULL);
    ERROR_CHECK(netfd, -1, "accept");

    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    /*监听集合*/
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);
    ERROR_CHECK(ret, -1, "epoll_ctl stdin");

    ev.events = EPOLLIN;
    ev.data.fd = netfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, &ev);
    ERROR_CHECK(ret, -1, "epoll_ctl netfd");

    char buf[4096] = {0};
    while (1) {
        /*就绪集合*/
        struct epoll_event readyEvents[MAXEVENTS];
        int readyNum = epoll_wait(epfd, readyEvents, MAXEVENTS, -1);
        ERROR_CHECK(readyNum, -1, "epoll_wait");
        for (int i = 0; i < readyNum; ++i) {
            if (readyEvents[i].data.fd == STDIN_FILENO) {
                memset(buf, 0, sizeof(buf));
                ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
                send(netfd, buf, sret, 0);
            } else if (readyEvents[i].data.fd == netfd) {
                memset(buf, 0, sizeof(buf));
                ssize_t sret = recv(netfd, buf, sizeof(buf), 0);
                if (sret == 0) {
                    // 客户端关闭连接
                    printf("client close connection\n");
                    close(netfd);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd,nullptr);
                    exit(0);
                }
                printf("buf = %s\n", buf);
            }
        }
    }

    close(epfd);
    close(sockfd);
    return 0;
}    