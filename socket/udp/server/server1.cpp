#include<func.h>

int main(int argc,char *argv[]){
    ARGC_CHECK(argc,3);
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"socket");

    struct sockaddr_in clientAddr;
    socklen_t socklen=sizeof(clientAddr);
    char buf[4096]={0};
    recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&clientAddr,&socklen);

    printf("buf = %s, ip =%s, port = %d\n",buf,inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));

    sendto(sockfd,"world",5,0,(struct sockaddr*)&clientAddr,sizeof(clientAddr));
    sendto(sockfd,"world",5,0,(struct sockaddr*)&clientAddr,sizeof(clientAddr));

    return 0;
}