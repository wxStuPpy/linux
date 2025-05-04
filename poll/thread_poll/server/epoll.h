#pragma once
#include "threadPool.h"

int epollAdd(int epfd,int fd);
int epollDel(int epfd,int fd);