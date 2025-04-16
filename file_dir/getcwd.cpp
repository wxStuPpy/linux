#include <func.h>
int main()
{
    char path[1024] = {0};
    char *p = getcwd(path, sizeof(path));
    ERROR_CHECK(p, nullptr, "getcwd");
    printf("cwd=%s\n",p);
    return 0;

    char *pp=getcwd(nullptr,0);//自动申请一片空间 需要释放
    free(pp);

    //int mkdir(const char *pathname, mode_t mode);c
    //创建一个权限为mode-umask的目录
    // int rmdir(const char *pathname);
}