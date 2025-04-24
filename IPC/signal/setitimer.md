在 Linux 系统编程里，`itimer` 一般和 `struct itimerval` 结构体以及相关的定时器函数关联，下面从多个方面详细介绍它。

### 1. `struct itimerval` 结构体
`itimer` 本质上对应的是 `struct itimerval` 结构体，该结构体用于设置和获取间隔定时器（Interval Timer）的参数。此结构体在 `<sys/time.h>` 头文件中定义，其具体结构如下：
```c
struct itimerval {
    struct timeval it_interval;  // 定时器的间隔时间
    struct timeval it_value;     // 第一次触发定时器的时间
};
```
#### 成员解释
 - **`it_value`**：它是 `struct timeval` 类型的结构体，用来设定定时器首次触发的时间。`struct timeval` 结构体包含两个成员 `tv_sec`（秒）和 `tv_usec`（微秒），意味着可以精确到微秒级别来设置首次触发时间。
 - **`it_interval`**：同样是 `struct timeval` 类型的结构体，用于设置定时器首次触发之后的间隔时间。也就是说，定时器首次触发后，会按照这个间隔时间周期性地触发。

### 2. 定时器类型
`itimer` 相关的定时器有三种类型，可通过 `setitimer` 函数来设置，分别为：
 - **`ITIMER_REAL`**：基于真实时间（墙上时间）进行计时，超时后会向进程发送 `SIGALRM` 信号。常用于实现程序的超时功能，例如网络请求设置超时时间。
 - **`ITIMER_VIRTUAL`**：基于进程的用户态时间进行计时，超时后会向进程发送 `SIGVTALRM` 信号。这种定时器只统计进程在用户态执行代码所花费的时间。
 - **`ITIMER_PROF`**：基于进程的用户态时间和内核态时间之和进行计时，超时后会向进程发送 `SIGPROF` 信号。可用于性能分析，统计进程在用户态和内核态总共花费的时间。

### 3. 相关函数
#### `setitimer` 函数
此函数用于设置定时器的参数，其原型如下：
```c
#include <sys/time.h>
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
```
 - **参数说明**：
    - `which`：指定定时器的类型，可取 `ITIMER_REAL`、`ITIMER_VIRTUAL` 或 `ITIMER_PROF`。
    - `new_value`：指向 `struct itimerval` 结构体的指针，用于设置新的定时器参数。
    - `old_value`：若不为 `NULL`，则会保存之前的定时器参数。
 - **返回值**：调用成功返回 0，失败返回 -1 并设置 `errno`。

#### `getitimer` 函数
该函数用于获取当前定时器的参数，原型如下：
```c
#include <sys/time.h>
int getitimer(int which, struct itimerval *value);
```
 - **参数说明**：
    - `which`：指定要获取的定时器类型。
    - `value`：指向 `struct itimerval` 结构体的指针，用于存储获取到的定时器参数。
 - **返回值**：调用成功返回 0，失败返回 -1 并设置 `errno`。
 