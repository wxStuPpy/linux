#pragma once
#include<func.h>

#define BUFSIZE 10240
#define DOWN_PATH "../src/Sdisk/"
#define SPOT_NUM 3

typedef struct{
	int Len;
    int ctl_code;
	char buf[BUFSIZE];
}Train_t;

typedef struct {
    char filename[30];
    int filesize;
    char md5sum[50];
}File_info;
typedef struct {
    int cfd;
    int state;
    char *p;
    int pos;
    File_info file;
    int sfd;
}CD_info;
typedef struct {
    int fd;
    int pos;
    char client_name[30];
    int code ;
    File_info file;
}DQ_buf;
typedef struct {
    int download_fd;
    int task_num;
    CD_info cdinfo[10];//发生变化必改状态位；
//    int s_fd[10];
}SD_info;
typedef struct {
    int fd;
    int state;
    int soketfd;
    int pos;
    char *p;
    char name[30];
    int code ;
    File_info file;
}UP_INFO;
typedef struct {
    struct sockaddr_in sfd_in[SPOT_NUM];
    File_info file;
}MQ_buf;
