#include<func.h>
int main(int argc,char *argv[]){
    ARGC_CHECK(argc,2);
    int ret=chmod(argv[1],0664);
    ERROR_CHECK(ret,-1,"chmod");
    return 0;
}