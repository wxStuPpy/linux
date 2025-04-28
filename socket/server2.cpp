#include<func.h>

int main(int argc, char *argv[]) {
    ARGC_CHECK(argc,3);
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    //能否重入*****
    int reuse=1;
    int ret=setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");
    // int setsockopt(int sockfd, int level, int optname,
    //     const void optval[.optlen],
    //     socklen_t optlen);
    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 50);
    ERROR_CHECK(ret, -1, "listen");

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t clientAddrLen = sizeof(clientAddr);
    int netfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    ERROR_CHECK(netfd, -1, "accept");

    printf("ip = %s port = %d\n",
           inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    fd_set rdset;
    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(netfd, &rdset);

        int fdNum = select(netfd + 1, &rdset, NULL, NULL, NULL);
        if (fdNum == -1) {
            perror("select");
            break;
        }

        char buf[4096] = {0};
        if (FD_ISSET(STDIN_FILENO, &rdset)) {
            memset(buf, 0, sizeof(buf));
            ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
            if (sret > 0) {
                buf[strcspn(buf, "\n")] = 0;
                sret = write(netfd, buf, strlen(buf));
                printf("server sent sret = %ld buf = %s\n", sret, buf);
            }
        }
        if (FD_ISSET(netfd, &rdset)) {
            memset(buf, 0, sizeof(buf));
            ssize_t sret = recv(netfd, buf, sizeof(buf), 0);
            if (sret > 0) {
                printf("server received sret = %ld buf = %s\n", sret, buf);
            } else if (sret == 0) {
                printf("Client closed connection\n");
                break;
            } else {
                perror("recv");
                break;
            }
        }
    }

    close(netfd);
    close(sockfd);
    return 0;
}