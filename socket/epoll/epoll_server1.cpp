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
    ev.data.fd=sockfd;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);
    int netfd=-1;
    char buf[4096]={0};
    while(1){
        struct epoll_event readyEvents[MAXEVENTS];
        int readyNum=epoll_wait(epfd,readyEvents,MAXEVENTS,-1);
        for(int i=0;i<readyNum;++i){
            if(readyEvents[i].data.fd==sockfd){
                epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,nullptr);
                netfd=accept(sockfd,nullptr,nullptr);
                ev.data.fd=netfd;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);
                ev.data.fd=STDIN_FILENO;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);
                printf("connected!\n");
            }
            else if(netfd!=-1&&readyEvents[i].data.fd==netfd){
                memset(buf,0,sizeof(buf));
                ssize_t sret=recv(netfd,buf,sizeof(buf),0);
                if(sret==0){
                    printf("client close connected\n");
                    ev.events=EPOLLIN;
                    ev.data.fd=sockfd;
                    epoll_ctl(epfd,EPOLL_CTL_DEL,STDIN_FILENO,nullptr);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd,nullptr);
                    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
                    close(netfd);
                    netfd=-1;
                    break;
                }
                printf("sret = %ld,buf = %s\n",sret,buf);
            }
            else if(netfd!=-1&&readyEvents[i].data.fd==STDIN_FILENO){
                memset(buf,0,sizeof(buf));
                ssize_t sret=read(STDIN_FILENO,buf,sizeof(buf));
                if(sret==0){
                    printf("kick client\n");
                    ev.events=EPOLLIN;
                    ev.data.fd=sockfd;
                    epoll_ctl(epfd,EPOLL_CTL_DEL,STDIN_FILENO,nullptr);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd,nullptr);
                    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
                    close(netfd);
                    netfd=-1;
                    break;
                }
                send(netfd,buf,strlen(buf),0);
            }
        }
    }
}