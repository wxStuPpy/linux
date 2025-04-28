#include<func.h>

int main(int argc,char *argv[]){
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr=inet_addr(argv[1]);

    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    int ret=bind(sockfd,(sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"bind");

    ret=listen(sockfd,50);
    ERROR_CHECK(ret,-1,"listen");

    struct sockaddr_in clientAddr;
    memset(&clientAddr,0,sizeof(clientAddr));
    socklen_t clientAddrLen=sizeof(clientAddr);
    int netfd=accept(sockfd,(sockaddr*)&clientAddr,&clientAddrLen);

    printf("ip = %s port = %d\n",
        inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));

    char buf[4096];
    ssize_t sret=recv(netfd,buf,sizeof(buf),0);
    printf("buf = %s\n",buf);
    sret=send(netfd,"world",5,0);
    return 0;
}