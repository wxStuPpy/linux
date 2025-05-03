#include<func.h>

int tcpInit(const char*ip,const char*port,int *psockfd){
    *psockfd=socket(AF_INET,SOCK_STREAM,0);
    int reuse=1;
    int ret=setsockopt(*psockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(port));
    inet_aton(ip,&serverAddr.sin_addr);

    ret=bind(*psockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"bind");

    ret=listen(*psockfd,50);
    ERROR_CHECK(ret,-1,"listen");
    return 0;
}