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
    int netfd = -1;
    while (1)
    {
        memcpy(&readySet, &monitorSet, sizeof(monitorSet));
        select(20, &readySet, nullptr, nullptr, nullptr);
        if (FD_ISSET(sockfd, &readySet))
        {
            netfd = accept(sockfd, nullptr, nullptr);
            printf("connected!\n");
            FD_CLR(sockfd, &monitorSet);
            FD_SET(STDIN_FILENO, &monitorSet);
            FD_SET(netfd, &monitorSet);
        }
        if (FD_ISSET(STDIN_FILENO, &readySet))
        {
            memset(buf, 0, sizeof(buf));
            ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
            if (sret > 0)
            {
                buf[strcspn(buf, "\n")] = 0;
                sret = write(netfd, buf, strlen(buf));
                printf("server sent sret = %ld buf = %s\n", sret, buf);
            }
            //踢走客户端
            else if(sret == 0){
                printf("kick client\n");
                FD_SET(sockfd, &monitorSet);
                FD_CLR(STDIN_FILENO, &monitorSet);
                FD_CLR(netfd, &monitorSet);
                close(netfd);
                netfd=-1;
            }
        }
        if (FD_ISSET(netfd, &readySet))
        {
            memset(buf, 0, sizeof(buf));
            ssize_t sret = recv(netfd, buf, sizeof(buf), 0);
            if (sret > 0)
            {
                printf("server received sret = %ld buf = %s\n", sret, buf);
            }
            else if (sret == 0)
            {
                printf("Client closed connection\n");
                FD_SET(sockfd, &monitorSet);
                FD_CLR(STDIN_FILENO, &monitorSet);
                FD_CLR(netfd, &monitorSet);
                close(netfd);
                netfd=-1;
            }
        }
    }

    close(netfd);
    close(sockfd);
    return 0;
}