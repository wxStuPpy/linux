#include<func.h>

#define MAXEVENTS 20

 /*struct epoll_event {
        uint32_t      events;  /* Epoll events 
        epoll_data_t  data;    /* User data variable 
    };
    union epoll_data {
        void     *ptr;
        int       fd;
        uint32_t  u32;
        uint64_t  u64;
    };*/

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
    int netfd=accept(sockfd,nullptr,nullptr);

    int epfd=epoll_create(1);
    /*int epoll_ctl (int __epfd, int __op, int __fd,
		      struct epoll_event *__event)*/

    /*监听集合*/
    struct epoll_event ev;
    ev.events=EPOLLIN;
    ev.data.fd=STDIN_FILENO;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);
    ERROR_CHECK(ret,-1,"epoll_ctl stdin");

    ev.events=EPOLLIN;
    ev.data.fd=netfd;
    int ret=epoll_ctl(epfd,netfd,STDIN_FILENO,&ev);
    ERROR_CHECK(ret,-1,"epoll_ctl netfd");
    
    while(1){
    /*就绪集合*/
        struct epoll_event readyEvents[MAXEVENTS];
        int readyNum=epoll_wait(epfd,readyEvents,MAXEVENTS,-1);
        for(int i=0;i<readyNum;++i){
            if(readyEvents[i].data.fd==STDIN_FILENO){

            }
            /*此处对比select可以使用else if因为会挨个遍历每个就绪的文件描述符*/
            else if(readyEvents[i].data.fd==netfd){

            }
        }
    }

}