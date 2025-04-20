#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    int fd1 = open(argv[1], O_RDWR | O_CREAT|O_APPEND, 0644);
    if(fd1 == -1) {
        perror("open failed");
        return 1;
    }
    
    int saved_stdout = dup(STDOUT_FILENO);  // 更安全的方式保存标准输出
    dup2(fd1, STDOUT_FILENO);
    printf("%s", "ckun");
    fflush(stdout);  // 确保输出被刷新
    
    dup2(saved_stdout, STDOUT_FILENO);
    printf("%s\n", "stdout");
    
    close(fd1);
    close(saved_stdout);
    return 0;
}