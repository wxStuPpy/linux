#include<func.h>
int main(int argc,char *argv[]){
    ARGC_CHECK(argc,2);
    DIR*dirp=opendir(argv[1]);
    ERROR_CHECK(dirp,nullptr,"opendir");
    struct dirent*pdirent;
    while((pdirent=readdir(dirp))!=nullptr){
        //文件名->路径
        char path[1024]={0};
        sprintf(path,"%s%s%s",argv[1],"/",pdirent->d_name);
        struct stat statbuf;
        int ret=stat(path,&statbuf);
        ERROR_CHECK(ret,-1,"stat");
        printf("%o %ld %d %d %ld %ld %s\n",
            statbuf.st_mode,statbuf.st_nlink,statbuf.st_uid,
            statbuf.st_gid,statbuf.st_size,statbuf.st_mtim.tv_sec,pdirent->d_name
        );//文件权限和类型
    }
    closedir(dirp);
} 