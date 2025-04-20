#include<func.h>

typedef struct train_S
{
    int length;
    char data[1024];
}train_t;


int main(int argc,char *argv[]){
    // ./trainA pipe1 file
    ARGC_CHECK(argc,3);
    int fdw=open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open pipe");

    train_t train;
    train.length=strlen(argv[2]);
    memcpy(train.data,argv[2],train.length);
    //发送文件名字段长度
    write(fdw,&train.length,sizeof(train.length));
    //发送文件名
    write(fdw,train.data,train.length);

    //发送文件内容字段长度
    int fdr=open(argv[2],O_RDONLY);
    //read会覆盖原来的data内容 从而只保留了文件中的内容
    ssize_t sret=read(fdr,train.data,sizeof(train.data));
    train.length=sret;
    write(fdw,&train.length,sizeof(train.length));
    //发送文件内容
    write(fdw,train.data,train.length);

    close(fdw);
    close(fdr);
}