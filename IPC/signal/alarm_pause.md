在 Linux 系统中，`alarm` 和 `pause` 也是与信号处理紧密相关的重要函数，下面结合之前介绍的信号函数，详细讲解这两个函数的功能和使用场景。

### 1. `alarm` 函数
#### 功能
`alarm` 函数用于设置一个定时器，当定时器超时后，会向当前进程发送 `SIGALRM` 信号。

#### 函数原型
```c
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
```
#### 参数说明
- `seconds`：指定定时器的超时时间，单位为秒。如果 `seconds` 为 0，则取消之前设置的定时器。

#### 返回值
- 如果之前已经设置过定时器，且定时器尚未超时，则返回之前定时器剩余的秒数。
- 如果之前没有设置过定时器，或者定时器已经超时，则返回 0。

#### 示例代码
```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigalrm_handler(int signum) {
    printf("Received SIGALRM signal\n");
}

int main() {
    signal(SIGALRM, sigalrm_handler);
    // 设置 5 秒后触发 SIGALRM 信号
    alarm(5); 
    printf("Waiting for alarm signal...\n");
    while (1) {
        // 主循环
    }
    return 0;
}
```
#### 代码解释
- 首先，使用 `signal` 函数为 `SIGALRM` 信号设置了一个处理函数 `sigalrm_handler`。
- 然后，调用 `alarm(5)` 设置一个 5 秒的定时器。当 5 秒过去后，系统会向当前进程发送 `SIGALRM` 信号，进而调用 `sigalrm_handler` 函数。

### 2. `pause` 函数
#### 功能
`pause` 函数会使当前进程挂起，直到收到一个信号。如果信号的处理函数返回，则 `pause` 函数会返回 -1，并将 `errno` 设置为 `EINTR`。

#### 函数原型
```c
#include <unistd.h>
int pause(void);
```
#### 返回值
- 始终返回 -1，并将 `errno` 设置为 `EINTR`。

#### 示例代码
```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigint_handler(int signum) {
    printf("Received SIGINT signal, exiting...\n");
}

int main() {
    signal(SIGINT, sigint_handler);
    printf("Process is paused, press Ctrl+C to continue...\n");
    // 进程挂起，等待信号
    pause(); 
    return 0;
}
```
#### 代码解释
- 首先，使用 `signal` 函数为 `SIGINT` 信号设置了一个处理函数 `sigint_handler`。
- 然后，调用 `pause` 函数使进程挂起。当用户按下 `Ctrl+C` 时，系统会向当前进程发送 `SIGINT` 信号，调用 `sigint_handler` 函数进行处理，之后 `pause` 函数返回 -1。

### 3. `alarm` 和 `pause` 结合使用
可以将 `alarm` 和 `pause` 结合使用，实现一个简单的超时机制。

#### 示例代码
```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigalrm_handler(int signum) {
    // 超时处理
    printf("Timeout occurred!\n");
}

int main() {
    signal(SIGALRM, sigalrm_handler);
    // 设置 3 秒的定时器
    alarm(3); 
    printf("Waiting for alarm or other signal...\n");
    // 进程挂起，等待信号
    pause(); 
    return 0;
}
```
#### 代码解释
- 首先，使用 `signal` 函数为 `SIGALRM` 信号设置了一个处理函数 `sigalrm_handler`。
- 然后，调用 `alarm(3)` 设置一个 3 秒的定时器。
- 接着，调用 `pause` 函数使进程挂起。如果 3 秒内没有收到其他信号，定时器超时后会发送 `SIGALRM` 信号，调用 `sigalrm_handler` 函数进行超时处理；如果在 3 秒内收到其他信号，`pause` 函数会被唤醒，继续执行后续代码。

综上所述，`alarm` 和 `pause` 函数为信号处理提供了更多的灵活性，可以用于实现定时器、超时机制等功能。结合之前介绍的信号函数，能够构建出更加复杂和健壮的信号处理系统。 