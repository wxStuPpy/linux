#include<func.h>
int main(int argc,char *argv[]){
    ARGC_CHECK(argc,2);
    DIR*dirp=opendir(argv[1]);
    ERROR_CHECK(dirp,nullptr,"opendir");
    struct dirent*pdirent;
    /*struct dirent
  {
#ifndef __USE_FILE_OFFSET64
    __ino_t d_ino;
    __off_t d_off;
#else
    __ino64_t d_ino;
    __off64_t d_off;
#endif
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];		// We must not include limits.h! //
  };*/
    while((pdirent=readdir(dirp))!=nullptr){
        printf("inode = %ld, reclen = %d, type= %d, name=%s\n",pdirent->d_ino,pdirent->d_reclen,pdirent->d_type,pdirent->d_name);
    }
    closedir(dirp);
}