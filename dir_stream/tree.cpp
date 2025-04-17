#include<func.h>
int DFSprint(char*dirpath,int width){
    DIR *dirp = opendir(dirpath);
    ERROR_CHECK(dirp, nullptr, "opendir");
    struct dirent *pdirent;
    while ((pdirent = readdir(dirp)) != nullptr){
        if(strcmp(pdirent->d_name,".")==0||
           strcmp(pdirent->d_name,"..")==0)
           {
            continue;
           }
        for(int i=0;i<width;++i){
            printf("-");
        }
        printf("%s\n",pdirent->d_name);
        if(pdirent->d_type==DT_DIR){
            char path[1024]={0};
            sprintf(path,"%s%s%s",dirpath,"/",pdirent->d_name);
            DFSprint(path,width+4);//递归调用
        }
    }
    return 0;
}
int main(int argc, char *argv[])
{
    ARGC_CHECK(argc, 2);
    DFSprint(argv[1],4);
}