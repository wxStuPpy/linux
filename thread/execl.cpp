#include<func.h>
int main(int argc,char *argv[]){
    printf("pid = %d\n",getpid());
    execl("./exec_add",//可执行函数路径
    "./exec_add","3","4",//argc参数
    nullptr);//以nullptr结尾
}