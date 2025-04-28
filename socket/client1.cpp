#include<func.h>

int main(int argc,char *argv[]){
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr=inet_addr(argv[1]);

    int sockfd=socket(AF_INET,SOCK_STREAM,0);

    int ret=connect(sockfd,(sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");

    send(sockfd,"hello",5,0);
    char buf[4096];
    ssize_t sret=recv(sockfd,buf,sizeof(buf),0);
    printf("buf = %s\n",buf);
    return 0;
}