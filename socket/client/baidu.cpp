#include<func.h>
int main(int argc, char *argv[])
{
    // HTTP协议的请求
    const char query[] = "GET / HTTP/1.0\r\n"
                         "Host: www.baidu.com\r\n"
                         "\r\n";
    const char hostname[] = "www.baidu.com";

    struct hostent *entry = gethostbyname(hostname); 
    if(entry == NULL){
        herror("gethostbyname");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr,entry->h_addr_list[0],sizeof(addr.sin_addr));

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    connect(sockfd,(struct sockaddr *)&addr, sizeof(addr));

    send(sockfd,query,strlen(query),0);

    char buf[4096] = {0};
    while(1){
        bzero(buf,4096);
        ssize_t sret = recv(sockfd,buf,4096,0);
        if(sret == 0){
            break;
        }
        printf("sret = %ld\n, buf = %s\n",sret, buf);
    }
    return 0;
}