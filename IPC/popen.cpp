#include<func.h>

/// @brief 创建一个子进程 并且父子进程之间存在管道
/// @param command 子进程执行的代码
/// @param type "W" 父是写端 子是读端 同时用管道读端取代子进程stdin
///             "r" 父是读端 子是写端 同时用管道读端取代子进程stdout
/// @return 返回管道一端的文件流(不是文件描述符 这是库函数)
/*FILE *popen(const char *command, const char *type);*/
int main(int argc,char *argv[]){
    FILE *fp=popen("./son_scanf","w");
    fwrite("1 6",1,3,fp);
    pclose(fp);
}