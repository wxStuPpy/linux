#include<func.h>

#define MAXEVENTS 20

int main(int argc,char *argv[]){
    ARGC_CHECK(argc,3);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    int reuse=1;
    int ret=setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");

    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr=inet_addr(argv[1]);
    ret=bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"bind");

    ret=listen(sockfd,40);
    ERROR_CHECK(ret,-1,"listen");

    int epfd=epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create server");

    struct epoll_event ev;
    ev.data.fd=STDIN_FILENO;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);
    ev.data.fd=sockfd;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);

    while(1){
        struct epoll_event readyEvents[MAXEVENTS];
        int readyNum=epoll_wait(epfd,readyEvents,MAXEVENTS,-1);
        for(int i=0;i<readyNum;++i){
            if(readyEvents[i].data.fd=sockfd){
                epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,nullptr);
                sockaddr_in clientAddr;
                socklen_t socklen;
                int netfd=accept(sockfd,(struct sockaddr*)&clientAddr,&socklen);
                ev.data.fd=netfd;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);
            }
        }
    }
}