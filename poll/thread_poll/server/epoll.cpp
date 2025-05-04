#include"epoll.h"

int epollAdd(int epfd,int fd){
    struct epoll_event ev;
    ev.data.fd=fd;
    ev.events=EPOLLIN;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);
    ERROR_CHECK(ret,-1,"epoll_ctl_add");
    return 0;
}
int epollDel(int epfd,int fd){
    int ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,nullptr);
    ERROR_CHECK(ret,-1,"epoll_ctl_del");
    return 0;
}