#include <func.h>

int setTypeMode(mode_t mode, char *str)
{
    switch (mode & S_IFMT)
    {
    case S_IFBLK:
        str[0] = 'b';
        break;
    case S_IFCHR:
        str[0] = 'c';
        break;
    case S_IFDIR:
        str[0] = 'd';
        break;
    case S_IFIFO:
        str[0] = 'p';
        break;
    case S_IFLNK:
        str[0] = 'l';
        break;
    case S_IFREG:
        str[0] = '-';
        break;
    case S_IFSOCK:
        str[0] = 's';
        break;
    default:
        str[0] = '?';
        break;
    }
    int i = 1 << 8; // read
    for (int j = 0; j < 3; ++j)
    {
        if (mode & i)
        {
            str[1 + j * 3] = 'r';
        }
        else
        {
            str[1 + j * 3] = '-';
        }
        i >>= 1;
        if (mode & i)
        {
            str[2 + j * 3] = 'w';
        }
        else
        {
            str[2 + j * 3] = '-';
        }
        i >>= 1;
        if (mode & i)
        {
            str[3 + j * 3] = 'x';
        }
        else
        {
            str[3 + j * 3] = '-';
        }
    }
    str[10] = '\0';
    return 0;
}

int getTimeType(time_t mtime,char*str){
    char monArr[][10] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    struct tm*tmVal=localtime(&mtime);
    int mon=tmVal->tm_mon;
    int mday=tmVal->tm_mday;
    int hour=tmVal->tm_hour;
    int min=tmVal->tm_min;
    sprintf(str,"%s %02d %02d:%02d",monArr[mon],mday,hour,min);
    return 0;
}

int main(int argc, char *argv[])
{
    ARGC_CHECK(argc, 2);
    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp, nullptr, "opendir");
    struct dirent *pdirent;
    while ((pdirent = readdir(dirp)) != nullptr)
    {
        // 文件名->路径
        char path[1024] = {0};
        sprintf(path, "%s%s%s", argv[1], "/", pdirent->d_name);
        struct stat statbuf;
        int ret = stat(path, &statbuf);
        ERROR_CHECK(ret, -1, "stat");
        char str[20] = {0}; //保存类型和权限
        char timeStr[20]={0};// 保存时间
        getTimeType(statbuf.st_mtime,timeStr);
        setTypeMode(statbuf.st_mode,str);
        printf("%-10s %3ld %-5s %-4s %6ld %s %s\n",
            str,statbuf.st_nlink,getpwuid(statbuf.st_uid)->pw_name,
            getgrgid(statbuf.st_gid)->gr_name,statbuf.st_size,
            timeStr,pdirent->d_name);
    }
    closedir(dirp);
}
