`msghdr` 结构体是在UNIX和类UNIX系统中用于表示消息头的结构体，定义在 `<sys/socket.h>` 头文件中，主要用于 `sendmsg()` 和 `recvmsg()` 函数来传递和接收消息。以下是其定义及成员说明：

```c
struct msghdr {
    void         *msg_name;       /* 指向地址结构体的指针，用于指定接收或发送的目标地址 */
    socklen_t     msg_namelen;    /* 地址结构体的长度 */
    struct iovec *msg_iov;        /* 指向iovec结构体数组的指针，用于指定消息数据的缓冲区 */
    int           msg_iovlen;     /* iovec结构体数组的元素个数 */
    void         *msg_control;    /* 指向辅助数据（控制信息）的缓冲区的指针 */
    socklen_t     msg_controllen; /* 辅助数据缓冲区的长度 */
    int           msg_flags;      /* 消息标志，用于指定消息的一些属性，如是否是紧急数据等 */
};
```

- `msg_name` 和 `msg_namelen`：对于面向连接的套接字，通常为 `NULL` 和 `0`；对于无连接的套接字，`msg_name` 指向一个包含目标地址的结构体（如 `sockaddr_in`），`msg_namelen` 是该结构体的长度，用于指定发送消息的目标地址或接收消息的源地址。
- `msg_iov` 和 `msg_iovlen`：`msg_iov` 是一个指向 `iovec` 结构体数组的指针，`iovec` 结构体定义了一个内存缓冲区的地址和长度。`msg_iovlen` 表示 `iovec` 结构体数组的元素个数，用于指定消息数据的缓冲区。这样可以实现分散/聚集 I/O，即可以将消息分散存储在多个不连续的缓冲区中，或者从多个不连续的缓冲区中聚集消息。
- `msg_control` 和 `msg_controllen`：用于传递辅助数据（也称为控制信息），如发送或接收带外数据、设置或获取套接字选项等。`msg_control` 指向一个缓冲区，`msg_controllen` 是该缓冲区的长度。
- `msg_flags`：是一些标志位，用于指定消息的一些属性，如 `MSG_OOB`（发送或接收带外数据）、`MSG_DONTROUTE`（不进行路由查找）等。这些标志可以在发送或接收消息时设置，以控制消息的传输方式。

`msghdr` 结构体提供了一种灵活的方式来传递和接收消息，包括消息数据、目标地址、辅助数据等信息，使得应用程序能够更方便地控制网络通信的细节。

`cmsghdr` 结构体在 Unix 和类 Unix 系统网络编程里扮演着重要角色，主要用于处理套接字通信中的辅助数据（也叫控制信息）。下面为你详细介绍：

### 定义
`cmsghdr` 结构体定义于 `<sys/socket.h>` 头文件，具体定义如下：
```c
struct cmsghdr {
    socklen_t cmsg_len;    /* 整个控制消息（包括 cmsghdr 结构体和实际控制数据）的长度，以字节为单位 */
    int       cmsg_level;  /* 表示控制消息所属的协议级别，例如 SOL_SOCKET 表示套接字级别的控制消息 */
    int       cmsg_type;   /* 表示控制消息的具体类型，不同的协议级别有不同的控制消息类型 */
    /* 实际的控制数据紧跟在 cmsghdr 结构体之后，这里没有显式声明 */
};
```

### 成员解释
- **`cmsg_len`**：此成员记录了整个控制消息的长度，涵盖 `cmsghdr` 结构体本身以及紧随其后的实际控制数据。该长度是从 `cmsghdr` 结构体的起始地址开始计算的。
- **`cmsg_level`**：表明控制消息所属的协议级别。常见的协议级别有 `SOL_SOCKET`（套接字级别），另外还可以是具体的协议级别，像 `IPPROTO_IP`（IPv4 协议）、`IPPROTO_IPV6`（IPv6 协议）等。
- **`cmsg_type`**：代表控制消息的具体类型。不同的协议级别对应着不同的控制消息类型。例如，在 `SOL_SOCKET` 级别下，`SCM_RIGHTS` 用于传递文件描述符；在 `IPPROTO_IP` 级别下，`IP_TTL` 用于设置 IP 数据报的生存时间。

### 辅助宏
为了更方便地操作 `cmsghdr` 结构体，系统提供了一系列辅助宏：
- **`CMSG_LEN(len)`**：用于计算包含长度为 `len` 的控制数据的 `cmsghdr` 结构体及其控制数据的总长度。
- **`CMSG_SPACE(len)`**：计算为包含长度为 `len` 的控制数据所需要的空间大小，这个值会考虑到内存对齐的问题。
- **`CMSG_FIRSTHDR(msg)`**：返回 `msghdr` 结构体中第一个 `cmsghdr` 结构体的指针，如果没有控制消息则返回 `NULL`。
- **`CMSG_NXTHDR(msg, cmsg)`**：返回 `msghdr` 结构体中 `cmsg` 之后的下一个 `cmsghdr` 结构体的指针，如果没有更多的控制消息则返回 `NULL`。
- **`CMSG_DATA(cmsg)`**：返回 `cmsghdr` 结构体中实际控制数据的指针。

### 使用示例
以下是一个简单的示例，展示了如何使用 `cmsghdr` 结构体来传递文件描述符：
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define CONTROL_LEN CMSG_LEN(sizeof(int))

void send_fd(int sockfd, int fd) {
    struct msghdr msg;
    struct iovec iov[1];
    char buf[1] = {0};
    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    struct cmsghdr *cmsg;
    char control[CONTROL_LEN];
    memset(control, 0, CONTROL_LEN);

    msg.msg_control = control;
    msg.msg_controllen = CONTROL_LEN;

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CONTROL_LEN;
    *(int *)CMSG_DATA(cmsg) = fd;

    sendmsg(sockfd, &msg, 0);
}

int main() {
    int sockfd[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd);

    int fd = open("test.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    send_fd(sockfd[0], fd);

    close(fd);
    close(sockfd[0]);
    close(sockfd[1]);

    return 0;
}
```
在这个示例中，`send_fd` 函数借助 `cmsghdr` 结构体将一个文件描述符通过套接字传递给另一个进程。

### 总结
`cmsghdr` 结构体为网络编程提供了一种传递辅助数据（如文件描述符、套接字选项等）的有效方式。通过合理运用 `cmsghdr` 结构体和相关辅助宏，开发者能够在套接字通信中传递更多的控制信息，从而实现更复杂的功能。 