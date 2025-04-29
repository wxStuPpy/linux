#include<func.h>

int main(int argc,char *argv[]){
    ARGC_CHECK(argc,3);
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr=inet_addr(argv[1]);
    /*第一次必须是客户端向服务端发消息 服务端此时不知道客户端的ip和port*/
    sendto(sockfd,"hello",5,0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    /*服务端获取到客户端ip和port后 可以相互之间通信*/
    char buf[4096]={0};
    sleep(1);
    //只会收到单次world 消息存在边界 
    recvfrom(sockfd,buf,sizeof(buf),0,nullptr,nullptr);

    printf("buf = %s\n",buf);

    return 0;
}