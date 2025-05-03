#include<func.h>

//1.0
int recvFile(int sockfd){
    
}

int main(int argc,char *argv[]){
    ARGC_CHECK(argc,3);
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr=inet_addr(argv[1]);

    int sockfd=socket(AF_INET,SOCK_STREAM,0);

    int ret=connect(sockfd,(sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");
    sleep(3);
    close(sockfd);
    return 0;
}