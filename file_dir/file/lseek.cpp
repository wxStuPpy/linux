#include<func.h>
int main(int argc,char*argv[]){
    ARGC_CHECK(argc,2);
    int fd=open(argv[1],O_RDWR);
    char buf[1];
    while(1){
    memset(buf,0,sizeof(buf));
    ssize_t sret=read(fd,buf,sizeof(buf));//指针后移一位
    if(sret==0){
        break;
    }
    if(buf[0]>='a'&&buf[0]<='z'){
        buf[0]-=32;
        lseek(fd,-1,SEEK_CUR);//当前指针前移一位
        write(fd,buf,sret);
    }
}
    close(fd);
    return 0;

}