#include <func.h>

void handler(int signum)
{
    time_t now = time(nullptr);
    printf("now = %s\n", ctime(&now));
}

int main(void)
{
    struct itimerval itimer;
    // 对 it_value 进行初始化
    itimer.it_value.tv_sec = 3;
    itimer.it_value.tv_usec = 0;
    // 对 it_interval 进行初始化
    itimer.it_interval.tv_sec = 1;
    itimer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &itimer, nullptr);
    signal(SIGALRM, handler);
    while (1)
    {
    }
    return 0;
}