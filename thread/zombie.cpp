#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // 子进程
        printf("Child process (PID: %d) is exiting.\n", getpid());
    } else {
        // 父进程
        printf("Parent process (PID: %d) is sleeping.\n", getpid());
        sleep(5);  // 父进程休眠 5 秒，不回收子进程资源
    }
    return 0;
}