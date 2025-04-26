#include <func.h>

void deletep1(void *arg)
{
    printf("delete p1\n");
    delete (int *)arg;
}

void deletep2(void *arg)
{
    printf("delete p2\n");
    delete (int *)arg;
}

void closefd(void *arg)
{
    printf("close fd\n");
    close(*(int *)arg);
}

void *fun(void *arg)
{
    /*push和pop必须成对出现 否则编译错误*/
    // 申请后马上压栈 pthread_cleanup_push本身不是取消点
    int *p1 = new int(1);
    pthread_cleanup_push(deletep1, p1);
    int *p2 = new int(2);
    pthread_cleanup_push(deletep2, p2);
    int fd = open("./cleanup", O_RDWR | O_CREAT | O_TRUNC, 0666);
    pthread_cleanup_push(closefd, &fd);
    // 后进后出
    /*closefd(&fd);
    deletep1(p2);
    deletep2(p1);*/
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    // 会弹栈 return不会弹栈 资源会泄露
    pthread_exit(nullptr);
}

int main(void)
{
    pthread_t tid;
    pthread_create(&tid, nullptr, fun, nullptr);
    pthread_join(tid, nullptr);
    return 0;
}