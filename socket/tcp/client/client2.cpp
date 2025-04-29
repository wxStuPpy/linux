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

    int ret = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "connect");

    fd_set rdset;
    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sockfd, &rdset);

        int fdNum = select(sockfd + 1, &rdset, NULL, NULL, NULL);
        if (fdNum == -1) {
            perror("select");
            break;
        }

        char buf[4096] = {0};
        if (FD_ISSET(STDIN_FILENO, &rdset)) {
            memset(buf, 0, sizeof(buf));
            ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
        }
        if (FD_ISSET(sockfd, &rdset)) {
            memset(buf, 0, sizeof(buf));
            ssize_t sret = recv(sockfd, buf, sizeof(buf), 0);
            if (sret > 0) {
                printf("client received sret = %ld buf = %s\n", sret, buf);
            } else if (sret == 0) {
                printf("Server closed connection\n");
                break;
            } else {
                perror("recv");
                break;
            }
        }
    }

    close(sockfd);
    return 0;
}