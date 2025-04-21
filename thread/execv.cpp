#include<func.h>
int main(int argc,char *argv[]){
    // int execv(const char *pathname, char *const argv[]);
    char *const argv_[]={"exec_add","3","4",nullptr};
    execv("exec_add",argv_);
    return 0;
}