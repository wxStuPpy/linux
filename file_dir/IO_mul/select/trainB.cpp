#include<func.h>

typedef struct train_S
{
    int length;
    char data[1024];
}train_t;


int main(int argc,char *argv[]){
    // ./trainB pipe1 
    ARGC_CHECK(argc,2);
    int fdr=open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open pipe");

    train_t train;
    //读取文件名字段长度
    read(fdr,&train.length,sizeof(train.length));
    //读取文件名
    read(fdr,train.data,train.length);
    char fileName[4096]={0};
    memcpy(fileName,train.data,train.length);

    //读取文件内容字段长度
    read(fdr,&train.length,sizeof(train.length));
    //读取文件内容
    read(fdr,train.data,train.length);
    char fileContent[4096]={0};
    memcpy(fileContent,train.data,train.length);

    printf("fileName = %s fileContent = %s\n",fileName,fileContent);

    close(fdr);
}