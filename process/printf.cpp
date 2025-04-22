#include<func.h>
int main(int argc,char*argv[]){
    /*没有\n  在行缓冲模式下没有刷新stdout缓冲区
    两个缓冲区都还残留了一份hello
    故会输出两个hellowworld*/
    printf("hello");
    fork();
    /*执行两次*/
    printf("world\n");
}