### 1. 信号集操作函数
信号集（`sigset_t`）是用来表示一组信号的数据类型，以下函数可对信号集进行操作：
- **`sigemptyset`**：把信号集初始化为空集。
```c
#include <signal.h>
int sigemptyset(sigset_t *set);
```
示例：
```c
sigset_t set;
sigemptyset(&set);
```
- **`sigfillset`**：将信号集初始化为包含所有信号的集合。
```c
#include <signal.h>
int sigfillset(sigset_t *set);
```
示例：
```c
sigset_t set;
sigfillset(&set);
```
- **`sigaddset`**：向信号集中添加指定信号。
```c
#include <signal.h>
int sigaddset(sigset_t *set, int signum);
```
示例：
```c
sigset_t set;
sigemptyset(&set);
sigaddset(&set, SIGINT);
```
- **`sigdelset`**：从信号集中删除指定信号。
```c
#include <signal.h>
int sigdelset(sigset_t *set, int signum);
```
示例：
```c
sigset_t set;
sigemptyset(&set);
sigaddset(&set, SIGINT);
sigdelset(&set, SIGINT);
```
- **`sigismember`**：检查指定信号是否在信号集中。
```c
#include <signal.h>
int sigismember(const sigset_t *set, int signum);
```
示例：
```c
sigset_t set;
sigemptyset(&set);
sigaddset(&set, SIGINT);
if (sigismember(&set, SIGINT)) {
    // 信号在集合中
}
```

### 2. 信号屏蔽操作函数
- **`sigprocmask`**：用于读取或修改进程的信号屏蔽字（即信号掩码）。
```c
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```
参数说明：
    - `how`：指定修改方式，有以下几种取值：
        - `SIG_BLOCK`：将 `set` 中的信号添加到当前信号屏蔽字中。
        - `SIG_UNBLOCK`：从当前信号屏蔽字中移除 `set` 中的信号。
        - `SIG_SETMASK`：将当前信号屏蔽字设置为 `set`。
    - `set`：要操作的信号集。
    - `oldset`：若不为 `NULL`，则保存修改前的信号屏蔽字。

示例：
```c
sigset_t set, oldset;
sigemptyset(&set);
sigaddset(&set, SIGINT);
// 阻塞 SIGINT 信号
sigprocmask(SIG_BLOCK, &set, &oldset); 
```

### 3. 未决信号检查函数
- **`sigpending`**：获取当前进程的未决信号集。
```c
#include <signal.h>
int sigpending(sigset_t *set);
```
示例：
```c
sigset_t pending;
sigpending(&pending);
if (sigismember(&pending, SIGINT)) {
    // SIGINT 信号未决
}
```

### 4. 信号捕获与处理函数
- **`signal`**：为指定信号设置信号处理函数。
```c
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```
示例：
```c
#include <stdio.h>
#include <signal.h>

void sigint_handler(int signum) {
    printf("Received SIGINT signal\n");
}

int main() {
    signal(SIGINT, sigint_handler);
    while (1) {
        // 主循环
    }
    return 0;
}
```
- **`sigaction`**：更健壮的信号处理函数，可替代 `signal`。
```c
#include <signal.h>
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```
示例：
```c
#include <stdio.h>
#include <signal.h>

void sigint_handler(int signum) {
    printf("Received SIGINT signal\n");
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    while (1) {
        // 主循环
    }
    return 0;
}
```

### 5. 信号暂停函数
- **`sigsuspend`**：临时修改信号屏蔽字，挂起进程直到捕获到一个信号。
```c
#include <signal.h>
int sigsuspend(const sigset_t *mask);
```
示例：
```c
sigset_t set, oldset;
sigemptyset(&set);
sigaddset(&set, SIGINT);
sigprocmask(SIG_BLOCK, &set, &oldset);
// 临时解除 SIGINT 信号的屏蔽并挂起进程
sigsuspend(&oldset); 
sigprocmask(SIG_SETMASK, &oldset, NULL);
```

### 6. 信号发送函数
- **`kill`**：向指定进程或进程组发送信号。
```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
```
示例：
```c
pid_t pid = 1234;
kill(pid, SIGTERM);
```
- **`raise`**：向当前进程发送信号。
```c
#include <signal.h>
int raise(int sig);
```
示例：
```c
raise(SIGTERM);
```

这些函数提供了丰富的信号处理功能，可满足不同场景下的信号管理需求。 